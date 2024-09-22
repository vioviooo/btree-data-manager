#include <filesystem>
#include <iostream>
#include <fstream>
#include <cstring>
#include <climits>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "../include/db_storage.h"
#include "../../../allocator/allocator_boundary_tags/include/allocator_boundary_tags.h"
#include "../../../allocator/allocator_buddies_system/include/allocator_buddies_system.h"
#include "../../../allocator/allocator_global_heap/include/allocator_global_heap.h"
#include "../../../allocator/allocator_red_black_tree/include/allocator_red_black_tree.h"
#include "../../../allocator/allocator_sorted_list/include/allocator_sorted_list.h"


#pragma region exceptions implementation

db_storage::setup_failure::setup_failure(
	std::string const &error_msg):
		logic_error(error_msg)
{ }

db_storage::load_failure::load_failure(
	std::string const &error_msg):
		logic_error(error_msg)
{ }

db_storage::invalid_struct_name_exception::invalid_struct_name_exception():
		logic_error("got invalid db structural element name")
{ }

db_storage::too_big_struct_name_exception::too_big_struct_name_exception():
		logic_error("got too big db structural element name")
{ }

db_storage::invalid_path_exception::invalid_path_exception():
		logic_error("got invalid path")
{ }

db_storage::too_big_path_exception::too_big_path_exception():
		logic_error("got too big path")
{ }

db_storage::insertion_of_existent_struct_attempt_exception::insertion_of_existent_struct_attempt_exception():
	logic_error("attempt to add existent struct to database")
{ }

db_storage::insertion_of_struct_failure::insertion_of_struct_failure():
	logic_error("failed to add struct")
{ }

db_storage::disposal_of_nonexistent_struct_attempt_exception::disposal_of_nonexistent_struct_attempt_exception():
	logic_error("attempt to dispose non-existent struct from database")
{ }

db_storage::insertion_of_existent_key_attempt_exception::insertion_of_existent_key_attempt_exception():
	logic_error("attempt to insert existent key into table")
{ }

db_storage::obtaining_of_nonexistent_key_attempt_exception::obtaining_of_nonexistent_key_attempt_exception():
	logic_error("attempt to obtain non-existent key from table")
{ }

db_storage::updating_of_nonexistent_key_attempt_exception::updating_of_nonexistent_key_attempt_exception():
	logic_error("attempt to update non-existent key in table")
{ }

db_storage::disposal_of_nonexistent_key_attempt_exception::disposal_of_nonexistent_key_attempt_exception():
	logic_error("attempt to dispose non-existent key from table")
{ }

#pragma endregion exceptions implementation

#pragma region collection implementation

db_storage::collection::collection(
	search_tree_variant tree_variant,
	db_storage::allocator_variant allocator_variant,
	allocator_with_fit_mode::fit_mode fit_mode,
	size_t t_for_b_trees):
		_tree_variant(tree_variant),
		_allocator_variant(allocator_variant),
		_fit_mode(fit_mode),
		_records_cnt(0),
		_disposed_cnt(0)
{
	switch (tree_variant)
	{
	case search_tree_variant::b:
		//break;
	case search_tree_variant::b_plus:
		//break;
	case search_tree_variant::b_star:
		//break;
	case search_tree_variant::b_star_plus:
		//break;
	default:
		try
		{
			_data = new b_tree<tkey, tdata *>(t_for_b_trees, tkey_comparer());
		}
		catch (std::bad_alloc const &)
		{
			throw;
		}
		break;
	}

    try
    {
        switch (_allocator_variant)
        {
            case allocator_variant::boundary_tags:
                _allocator = std::make_shared<allocator_boundary_tags>(1 << 22, nullptr, nullptr, _fit_mode);
                break;
            case allocator_variant::buddy_system:
                _allocator = std::make_shared<allocator_buddies_system>(22, nullptr, nullptr, _fit_mode);
                break;
            case allocator_variant::global_heap:
                _allocator = std::make_shared<allocator_global_heap>();
                break;
            case allocator_variant::red_black_tree:
                _allocator = std::make_shared<allocator_red_black_tree>(1 << 22, nullptr, nullptr, _fit_mode);
                break;
            case allocator_variant::sorted_list:
                _allocator = std::make_shared<allocator_sorted_list>(1 << 22, nullptr, nullptr, _fit_mode);
                break;
        }
    }
    catch (std::bad_alloc const &)
    {
        delete _data;
        throw;
    }
}

db_storage::collection::~collection()
{
	clear();
}

db_storage::collection::collection(
	collection const &other)
{
	copy_from(other);
}

db_storage::collection &db_storage::collection::operator=(
	collection const &other)
{
	if (this != &other)
	{
		clear();
		copy_from(other);
	}
	
	return *this;
}

db_storage::collection::collection(
	collection &&other) noexcept
{
	move_from(std::move(other));
}

db_storage::collection &db_storage::collection::operator=(
	db_storage::collection &&other) noexcept
{
	if (this != &other)
	{
		clear();
		move_from(std::move(other));
	}

	return *this;
}

void db_storage::collection::insert(
	tkey const &key,
	tvalue const &value,
	std::string const &path)
{
	collect_garbage(path);
	
	tdata *data = nullptr;
	
	try
	{
		if (get_instance()->_mode == mode::file_system)
		{
			data = reinterpret_cast<file_tdata *>(allocate_with_guard(sizeof(file_tdata), 1));
			allocator::construct(reinterpret_cast<file_tdata *>(data));
		}
		else
		{
			data = reinterpret_cast<ram_tdata *>(allocate_with_guard(sizeof(ram_tdata), 1));
			allocator::construct(reinterpret_cast<ram_tdata *>(data), value);
		}
	}
	catch (std::bad_alloc const &)
	{
		deallocate_with_guard(data);
		throw;
		// TODO
	}
	
	try
	{
		if (get_instance()->_mode == mode::file_system)
		{
			reinterpret_cast<file_tdata *>(data)->serialize(path, key, value);
		}
		
		_data->insert(key, data);
	}
	catch (std::ios::failure const &)
	{
		allocator::destruct(data);
		deallocate_with_guard(data);
		throw std::ios::failure("Failed to write data");
	}
	catch (search_tree<tkey, tdata *>::insertion_of_existent_key_attempt_exception_exception const &)
	{
		allocator::destruct(data);
		deallocate_with_guard(data);
		throw db_storage::insertion_of_existent_key_attempt_exception();
		// TODO
	}
	
	++_records_cnt;
}

void db_storage::collection::insert(
	tkey const &key,
	tvalue &&value,
	std::string const &path)
{
	collect_garbage(path);
	
	tdata *data = nullptr;
	
	try
	{
		if (get_instance()->_mode == mode::file_system)
		{
			data = reinterpret_cast<file_tdata *>(allocate_with_guard(sizeof(file_tdata), 1));
			allocator::construct(reinterpret_cast<file_tdata *>(data));
		}
		else
		{
			data = reinterpret_cast<ram_tdata *>(allocate_with_guard(sizeof(ram_tdata), 1));
			allocator::construct(reinterpret_cast<ram_tdata *>(data), std::move(value));
		}
	}
	catch (std::bad_alloc const &)
	{
		deallocate_with_guard(data);
		throw;
		// TODO
	}
	
	try
	{
		if (get_instance()->_mode == mode::file_system)
		{
			reinterpret_cast<file_tdata *>(data)->serialize(path, key, value);
		}
		
		_data->insert(key, data);
	}
	catch (std::ios::failure const &)
	{
		allocator::destruct(data);
		deallocate_with_guard(data);
		throw std::ios::failure("Failed to write data");
	}
	catch (search_tree<tkey, tdata *>::insertion_of_existent_key_attempt_exception_exception const &)
	{
		allocator::destruct(data);
		deallocate_with_guard(data);
		throw db_storage::insertion_of_existent_key_attempt_exception();
		// TODO
	}
	
	++_records_cnt;
}

void db_storage::collection::update(
	tkey const &key,
	tvalue const &value,
	std::string const &path)
{
	collect_garbage(path);
	
	tdata *data = nullptr;
	
	try
	{
		if (get_instance()->_mode == mode::file_system)
		{
			data = reinterpret_cast<file_tdata *>(allocate_with_guard(sizeof(file_tdata), 1));
			allocator::construct(reinterpret_cast<file_tdata *>(data));
		}
		else
		{
			data = reinterpret_cast<ram_tdata *>(allocate_with_guard(sizeof(ram_tdata), 1));
			allocator::construct(reinterpret_cast<ram_tdata *>(data), value);
		}
	}
	catch (std::bad_alloc const &)
	{
		deallocate_with_guard(data);
		throw;
		// TODO
	}
	
	try
	{
		if (get_instance()->_mode == mode::file_system)
		{
			reinterpret_cast<file_tdata *>(data)->serialize(path, key, value);
		}
		
		_data->update(key, data);
	}
	catch (std::ios::failure const &)
	{
		allocator::destruct(data);
		deallocate_with_guard(data);
		throw std::ios::failure("Failed to write data");
	}
	catch (search_tree<tkey, tdata *>::updating_of_nonexistent_key_attempt_exception const &)
	{
		allocator::destruct(data);
		deallocate_with_guard(data);
		throw db_storage::updating_of_nonexistent_key_attempt_exception();
		// TODO
	}
}

void db_storage::collection::update(
	tkey const &key,
	tvalue &&value,
	std::string const &path)
{
	collect_garbage(path);
	
	tdata *data = nullptr;
	
	try
	{
		if (get_instance()->_mode == mode::file_system)
		{
			data = reinterpret_cast<file_tdata *>(allocate_with_guard(sizeof(file_tdata), 1));
			allocator::construct(reinterpret_cast<file_tdata *>(data));
		}
		else
		{
			data = reinterpret_cast<ram_tdata *>(allocate_with_guard(sizeof(ram_tdata), 1));
			allocator::construct(reinterpret_cast<ram_tdata *>(data), std::move(value));
		}
	}
	catch (std::bad_alloc const &)
	{
		deallocate_with_guard(data);
		throw;
		// TODO
	}
	
	try
	{
		if (get_instance()->_mode == mode::file_system)
		{
			reinterpret_cast<file_tdata *>(data)->serialize(path, key, value);
		}
		
		_data->update(key, data);
	}
	catch (std::ios::failure const &)
	{
		allocator::destruct(data);
		deallocate_with_guard(data);
		throw std::ios::failure("Failed to write data");
	}
	catch (search_tree<tkey, tdata *>::updating_of_nonexistent_key_attempt_exception const &)
	{
		allocator::destruct(data);
		deallocate_with_guard(data);
		throw db_storage::updating_of_nonexistent_key_attempt_exception();
		// TODO
	}
}

tvalue db_storage::collection::dispose(
	tkey const &key,
	std::string const &path)
{
	tdata *data = nullptr;
	tvalue value;
	
	try
	{
		data = _data->dispose(key);
	}
	catch (search_tree<tkey, tdata *>::disposal_of_nonexistent_key_attempt_exception)
	{
		throw db_storage::disposal_of_nonexistent_key_attempt_exception();
		// TODO
	}
	
	if (get_instance()->_mode == mode::file_system)
	{
		try
		{
			value = dynamic_cast<file_tdata *>(data)->deserialize(path);
		}
		catch (std::ios::failure const &)
		{
			allocator::destruct(data);
			deallocate_with_guard(data);
			throw std::ios::failure("Failed to parse disposed data");
		}
	}
	else
	{
		value = dynamic_cast<ram_tdata *>(data)->value;
	}
	
	allocator::destruct(data);
	deallocate_with_guard(data);
	
	--_records_cnt;
	++_disposed_cnt;
	
	return value;
};

tvalue db_storage::collection::obtain(
	tkey const &key,
	std::string const &path)
{
	collect_garbage(path);
	
	tdata *data = nullptr;
	
	try
	{
		data = _data->obtain(key);
	}
	catch (search_tree<tkey, tdata *>::obtaining_of_nonexistent_key_attempt_exception)
	{
		throw db_storage::obtaining_of_nonexistent_key_attempt_exception();
		// TODO
	}
	
	if (get_instance()->_mode == mode::file_system)
	{
		try
		{
			return dynamic_cast<file_tdata *>(data)->deserialize(path);
		}
		catch (std::ios::failure const &)
		{
			throw std::ios::failure("Failed to read data");
		}
	}
	else
	{
		return dynamic_cast<ram_tdata *>(data)->value;
	}
};

std::vector<std::pair<tkey, tvalue>> db_storage::collection::obtain_between(
	tkey const &lower_bound,
	tkey const &upper_bound,
	bool lower_bound_inclusive,
	bool upper_bound_inclusive,
	std::string const &path)
{
	collect_garbage(path);
	
	std::vector<typename associative_container<tkey, tdata *>::key_value_pair> data_vec =
			_data->obtain_between(lower_bound, upper_bound, lower_bound_inclusive, upper_bound_inclusive);
	
	std::vector<std::pair<tkey, tvalue>> value_vec;
	value_vec.reserve(data_vec.size());
	
	for (auto kvp : data_vec)
	{
		if (get_instance()->_mode == mode::file_system)
		{
			try
			{
				value_vec.emplace_back(kvp.key, dynamic_cast<file_tdata *>(kvp.value)->deserialize(path));
			}
			catch (std::ios::failure const &)
			{
				throw std::ios::failure("Failed to read data");
			}
		}
		else
		{
			value_vec.emplace_back(kvp.key, dynamic_cast<ram_tdata *>(kvp.value)->value);
		}
	}
	
	return value_vec;
}

std::pair<tkey, tvalue> db_storage::collection::obtain_max(
	std::string const &path)
{
	collect_garbage(path);
	
	tkey key;
	tdata *data = nullptr;
	
	switch (_tree_variant)
	{
		default:
		{
			b_tree<tkey, tdata *> *tree = dynamic_cast<b_tree<tkey, tdata *> *>(_data);
			
			auto iter = tree->rbegin_infix();
			auto iter_end = tree->rend_infix();
			
			if (iter == iter_end)
			{
				throw db_storage::obtaining_of_nonexistent_key_attempt_exception();
			}
			
			key = std::get<2>(*iter);
			data = std::get<3>(*iter);
		}
			
	}
	
	if (get_instance()->_mode == mode::file_system)
	{
		try
		{
			return make_pair(key, dynamic_cast<file_tdata *>(data)->deserialize(path));
		}
		catch (std::ios::failure const &)
		{
			throw std::ios::failure("Failed to read data");
		}
	}
	else
	{
		return make_pair(key, dynamic_cast<ram_tdata *>(data)->value);
	}
};

std::pair<tkey, tvalue> db_storage::collection::obtain_next(
	std::string const &path,
		tkey const &key)
{
	collect_garbage(path);
	
	tkey next_key;
	tdata *data = nullptr;
	
	switch (_tree_variant)
	{
		default:
		{
			b_tree<tkey, tdata *> *tree = dynamic_cast<b_tree<tkey, tdata *> *>(_data);
			
			auto iter = tree->begin_infix();
			auto iter_end = tree->end_infix();
			
			while (iter != iter_end && tkey_comparer()(key, std::get<2>(*iter)))
			{
				++iter;
			}
			
			if (iter == iter_end)
			{
				throw db_storage::obtaining_of_nonexistent_key_attempt_exception();
			}
			
			next_key = std::get<2>(*iter);
			data = std::get<3>(*iter);
			
			if (++iter != iter_end)
			{
				next_key = std::get<2>(*iter);
				data = std::get<3>(*iter);
			}
			
		}
			
	}
	
	if (get_instance()->_mode == mode::file_system)
	{
		try
		{
			return make_pair(next_key, dynamic_cast<file_tdata *>(data)->deserialize(path));
		}
		catch (std::ios::failure const &)
		{
			throw std::ios::failure("Failed to read data");
		}
	}
	else
	{
		return make_pair(next_key, dynamic_cast<ram_tdata *>(data)->value);
	}
};

std::pair<tkey, tvalue> db_storage::collection::obtain_min(
	std::string const &path)
{
	collect_garbage(path);
	
	tkey key;
	tdata *data = nullptr;
	
	switch (_tree_variant)
	{
		default:
		{
			b_tree<tkey, tdata *> *tree = dynamic_cast<b_tree<tkey, tdata *> *>(_data);
			
			auto iter = tree->begin_infix();
			auto iter_end = tree->end_infix();
			
			if (iter == iter_end)
			{
				throw db_storage::obtaining_of_nonexistent_key_attempt_exception();
			}
			
			key = std::get<2>(*iter);
			data = std::get<3>(*iter);
		}
			
	}
	
	if (get_instance()->_mode == mode::file_system)
	{
		try
		{
			return make_pair(key, dynamic_cast<file_tdata *>(data)->deserialize(path));
		}
		catch (std::ios::failure const &)
		{
			throw std::ios::failure("Failed to read data");
		}
	}
	else
	{
		return make_pair(key, dynamic_cast<ram_tdata *>(data)->value);
	}
};

size_t db_storage::collection::get_records_cnt()
{
	return _records_cnt;
}

void db_storage::collection::load(
	tkey const &key,
	tvalue &&value,
	std::string const &path,
	long file_pos)
{
	tdata *data = nullptr;
	
	try
	{
		if (get_instance()->_mode == mode::file_system)
		{
			data = reinterpret_cast<file_tdata *>(allocate_with_guard(sizeof(file_tdata), 1));
			allocator::construct(reinterpret_cast<file_tdata *>(data), file_pos);
		}
		else
		{
			data = reinterpret_cast<ram_tdata *>(allocate_with_guard(sizeof(ram_tdata), 1));
			allocator::construct(reinterpret_cast<ram_tdata *>(data), std::move(value));
		}
	}
	catch (std::bad_alloc const &)
	{
		deallocate_with_guard(data);
		throw;
		// TODO
	}
	
	try
	{
		_data->insert(key, data);
	}
	catch (search_tree<tkey, tdata *>::insertion_of_existent_key_attempt_exception_exception const &)
	{
		deallocate_with_guard(data);
		throw db_storage::insertion_of_existent_key_attempt_exception();
		// TODO
	}
	
	++_records_cnt;
}

void db_storage::collection::consolidate(
	std::string const &path)
{
	if (get_instance()->_mode == mode::in_memory_cache)
	{
		return;
	}
	
	std::string tmp_dir_path = extra_utility::make_path({path, "tmp"});
	std::string data_path = extra_utility::make_path({path, std::to_string(get_instance()->_id)});
	std::string tmp_path = extra_utility::make_path({path, "tmp", std::to_string(get_instance()->_id)});
	
	if (access(data_path.c_str(), F_OK) == -1)
	{
		return;
	}
	
	mkdir(tmp_dir_path.c_str(), 0777);
	
	switch (_tree_variant)
	{
		case search_tree_variant::b:
			//break;
		case search_tree_variant::b_plus:
			//break;
		case search_tree_variant::b_star:
			//break;
		case search_tree_variant::b_star_plus:
			//break;
		default:
		{
			long pos = 0;
			auto iter = dynamic_cast<b_tree<tkey, tdata *> *>(_data)->begin_infix();
			auto iter_end = dynamic_cast<b_tree<tkey, tdata *> *>(_data)->end_infix();
			
			std::fstream tmp_stream(tmp_path, std::ios::out | std::ios::trunc);
			tmp_stream.close();
			
			for (; iter != iter_end; ++iter)
			{
				tkey key = std::get<2>(*iter);
				file_tdata *data = dynamic_cast<file_tdata *>(std::get<3>(*iter));
				tvalue value = data->deserialize(data_path);
				data->serialize(tmp_path, key, value, true);
			}
			tmp_stream.flush();
			
			tmp_stream.open(tmp_path, std::ios::in | std::ios::binary);
			if (!tmp_stream.is_open())
			{
				throw std::ios::failure("Failed to open tmp fail");
			}
			
			std::fstream data_stream(data_path, std::ios::out | std::ios::binary | std::ios::trunc);
			if (!data_stream.is_open())
			{
				throw std::ios::failure("Failed to open data file");
			}
			
			(data_stream << tmp_stream.rdbuf()).flush();
			std::remove(tmp_path.c_str());
		}
	}
}


void db_storage::collection::clear()
{
	delete _data;
	_data = nullptr;
};

void db_storage::collection::copy_from(
	collection const &other)
{
	switch (_tree_variant = other._tree_variant)
	{
	case search_tree_variant::b:
		//break;
	case search_tree_variant::b_plus:
		//break;
	case search_tree_variant::b_star:
		//break;
	case search_tree_variant::b_star_plus:
		//break;
	default:
		try
		{
			_data = new b_tree<tkey, tdata *>(
				*dynamic_cast<b_tree<tkey, tdata *> *>(other._data));
		}
		catch (std::bad_alloc const &)
		{
			throw;
		}
		break;
	}
};

void db_storage::collection::move_from(
	collection &&other)
{
	switch (_tree_variant = other._tree_variant)
	{
	case search_tree_variant::b:
		//break;
	case search_tree_variant::b_plus:
		//break;
	case search_tree_variant::b_star:
		//break;
	case search_tree_variant::b_star_plus:
		//break;
	default:
		try
		{
			_data = new b_tree<tkey, tdata *>(
				std::move(*dynamic_cast<b_tree<tkey, tdata *> *>(other._data)));
		}
		catch (std::bad_alloc const &)
		{
			// TODO LOG
			throw;
		}
		break;
	}
	
	other._data = nullptr;
	
	// TODO ALLOCATORS
};

void db_storage::collection::collect_garbage(
	std::string const &path)
{
	if (_disposed_cnt > 0.35 * _records_cnt)
	{
		consolidate(path);
		_disposed_cnt = 0;
	}
}

[[nodiscard]] inline allocator *db_storage::collection::get_allocator() const
{
	return _allocator.get();
}

#pragma endregion collection implementation

#pragma region scheme implementation

db_storage::schema::schema(
	search_tree_variant tree_variant,
	size_t t_for_b_trees)
{
	switch (tree_variant)
	{
	case search_tree_variant::b:
		//break;
	case search_tree_variant::b_plus:
		//break;
	case search_tree_variant::b_star:
		//break;
	case search_tree_variant::b_star_plus:
		//break;
	default:
		try
		{
			_collections = new b_tree<std::string, collection>(t_for_b_trees);
		}
		catch (std::bad_alloc const &)
		{
			// TODO LOG
			throw;
		}
		break;
	}
}

db_storage::schema::~schema()
{
	clear();
}

db_storage::schema::schema(
	db_storage::schema const &other)
{
	copy_from(other);
}

db_storage::schema &db_storage::schema::operator=(
	db_storage::schema const &other)
{
	if (this != &other)
	{
		clear();
		copy_from(other);
	}
	
	return *this;
}

db_storage::schema::schema(
	db_storage::schema &&other) noexcept
{
	move_from(std::move(other));
}

db_storage::schema &db_storage::schema::operator=(
	db_storage::schema &&other) noexcept
{
	if (this != &other)
	{
		clear();
		move_from(std::move(other));
	}
	
	return *this;
}

void db_storage::schema::add(
	std::string const &collection_name,
	search_tree_variant tree_variant,
	db_storage::allocator_variant allocator_variant,
	allocator_with_fit_mode::fit_mode fit_mode,
	size_t t_for_b_trees)
{
	try
	{
		_collections->insert(collection_name, collection(tree_variant, allocator_variant, fit_mode, t_for_b_trees));
	}
	catch (search_tree<std::string, collection>::insertion_of_existent_key_attempt_exception_exception const &)
	{
		throw db_storage::insertion_of_existent_struct_attempt_exception();
		// TODO;
	}
}

void db_storage::schema::dispose(
	std::string const &collection_name)
{
	try
	{
		_collections->dispose(collection_name);
	}
	catch (search_tree<std::string, collection>::disposal_of_nonexistent_key_attempt_exception const &)
	{
		throw db_storage::disposal_of_nonexistent_struct_attempt_exception();
		// TODO;
	}
}

void db_storage::schema::consolidate(
	std::string const &path)
{
	switch (_tree_variant)
	{
	case search_tree_variant::b:
		//break;
	case search_tree_variant::b_plus:
		//break;
	case search_tree_variant::b_star:
		//break;
	case search_tree_variant::b_star_plus:
		//break;
	default:
		auto iter = dynamic_cast<b_tree<std::string, collection> *>(_collections)->begin_infix();
		auto iter_end = dynamic_cast<b_tree<std::string, collection> *>(_collections)->end_infix();
		
		for (; iter != iter_end; ++iter)
		{
			std::get<3>(*iter).consolidate(extra_utility::make_path({path, std::get<2>(*iter)}));
		}
	}
}

db_storage::collection &db_storage::schema::obtain(
	std::string const &collection_name)
{
	try
	{
		return _collections->obtain(collection_name);
	}
	catch (search_tree<std::string, collection>::obtaining_of_nonexistent_key_attempt_exception const &)
	{
		throw db_storage::invalid_path_exception();
		// TODO;
	}
}

void db_storage::schema::clear()
{
	delete _collections;
	_collections = nullptr;
}

void db_storage::schema::copy_from(
	schema const &other)
{
	switch (_tree_variant = other._tree_variant)
	{
	case search_tree_variant::b:
		//break;
	case search_tree_variant::b_plus:
		//break;
	case search_tree_variant::b_star:
		//break;
	case search_tree_variant::b_star_plus:
		//break;
	default:
		try
		{
			_collections = new b_tree<std::string, collection>(
				*dynamic_cast<b_tree<std::string, collection> *>(other._collections));
		}
		catch (std::bad_alloc const &)
		{
			// TODO LOG
			throw;
		}
		break;
	}
}

void db_storage::schema::move_from(
	schema &&other)
{
	switch (_tree_variant = other._tree_variant)
	{
	case search_tree_variant::b:
		//break;
	case search_tree_variant::b_plus:
		//break;
	case search_tree_variant::b_star:
		//break;
	case search_tree_variant::b_star_plus:
		//break;
	default:
		try
		{
			_collections = new b_tree<std::string, collection>(
				std::move(*dynamic_cast<b_tree<std::string, collection> *>(other._collections)));
		}
		catch (std::bad_alloc const &)
		{
			// TODO LOG
			throw;
		}
		break;
	}
	
	other._collections = nullptr;
}	

#pragma endregion scheme implementation

#pragma region pool implementation

db_storage::pool::pool(
	search_tree_variant tree_variant,
	size_t t_for_b_trees)
{
	switch (tree_variant)
	{
	case search_tree_variant::b:
		//break;
	case search_tree_variant::b_plus:
		//break;
	case search_tree_variant::b_star:
		//break;
	case search_tree_variant::b_star_plus:
		//break;
	default:
		try
		{
			_schemas = new b_tree<std::string, schema>(t_for_b_trees);
		}
		catch (std::bad_alloc const &)
		{
			// TODO LOG
			throw;
		}
		break;
	}
}

db_storage::pool::~pool()
{
	clear();
}

db_storage::pool::pool(
	pool const &other)
{
	copy_from(other);
}

db_storage::pool &db_storage::pool::operator=(
	db_storage::pool const &other)
{
	if (this != &other)
	{
		clear();
		copy_from(other);
	}
	
	return *this;
}

db_storage::pool::pool(
	db_storage::pool &&other) noexcept
{
	move_from(std::move(other));
}

db_storage::pool &db_storage::pool::operator=(
	db_storage::pool &&other) noexcept
{
	if (this != &other)
	{
		clear();
		move_from(std::move(other));
	}
	
	return *this;
}

void db_storage::pool::add(
	std::string const &schema_name,
	search_tree_variant tree_variant,
	size_t t_for_b_trees)
{
	try
	{
		_schemas->insert(schema_name, schema(tree_variant, t_for_b_trees));
	}
	catch (search_tree<std::string, schema>::insertion_of_existent_key_attempt_exception_exception const &)
	{
		throw db_storage::insertion_of_existent_struct_attempt_exception();
		// TODO;
	}
}

void db_storage::pool::dispose(
	std::string const &schema_name)
{
	try
	{
		_schemas->dispose(schema_name);
	}
	catch (search_tree<std::string, schema>::disposal_of_nonexistent_key_attempt_exception const &)
	{
		throw db_storage::disposal_of_nonexistent_struct_attempt_exception();
		// TODO;
	}
}

db_storage::schema &db_storage::pool::obtain(
	std::string const &schema_name)
{
	try
	{
		return _schemas->obtain(schema_name);
	}
	catch (search_tree<std::string, schema>::obtaining_of_nonexistent_key_attempt_exception const &)
	{
		throw db_storage::invalid_path_exception();
		// TODO;
	}
}

void db_storage::pool::consolidate(
	std::string const &path)
{
	switch (_tree_variant)
	{
	case search_tree_variant::b:
		//break;
	case search_tree_variant::b_plus:
		//break;
	case search_tree_variant::b_star:
		//break;
	case search_tree_variant::b_star_plus:
		//break;
	default:
		auto iter = dynamic_cast<b_tree<std::string, schema> *>(_schemas)->begin_infix();
		auto iter_end = dynamic_cast<b_tree<std::string, schema> *>(_schemas)->end_infix();
		
		for (; iter != iter_end; ++iter)
		{
			std::get<3>(*iter).consolidate(extra_utility::make_path({path, std::get<2>(*iter)}));
		}
	}
}

void db_storage::pool::clear()
{
	delete _schemas;
	_schemas = nullptr;
}

void db_storage::pool::copy_from(
	db_storage::pool const &other)
{
	switch (_tree_variant = other._tree_variant)
	{
	case search_tree_variant::b:
		//break;
	case search_tree_variant::b_plus:
		//break;
	case search_tree_variant::b_star:
		//break;
	case search_tree_variant::b_star_plus:
		//break;
	default:
		try
		{
			_schemas = new b_tree<std::string, schema>(
				*dynamic_cast<b_tree<std::string, schema> *>(other._schemas));
		}
		catch (std::bad_alloc const &)
		{
			// TODO LOG
			throw;
		}
		break;
	}
}

void db_storage::pool::move_from(
	db_storage::pool &&other)
{
	switch (_tree_variant = other._tree_variant)
	{
	case search_tree_variant::b:
		//break;
	case search_tree_variant::b_plus:
		//break;
	case search_tree_variant::b_star:
		//break;
	case search_tree_variant::b_star_plus:
		//break;
	default:
		try
		{
			_schemas = new b_tree<std::string, schema>(
				std::move(*dynamic_cast<b_tree<std::string, schema> *>(other._schemas)));
		}
		catch (std::bad_alloc const &)
		{
			// TODO LOG
			throw;
		}
		break;
	}
	
	other._schemas = nullptr;
}

#pragma endregion pool implementation



#pragma region db storage instance getter and constructor implementation

db_storage *db_storage::get_instance()
{
	static auto *instance = new db_storage();
	return instance;
}

db_storage::db_storage():
	_id(0),
	_mode(mode::uninitialized),
	_pools(8)
{ }

#pragma endregion db storage instance getter and constructor implementation

#pragma region db storage public operations implementation

size_t db_storage::get_id()
{
	return _id;
}

db_storage *db_storage::setup(
	size_t id,
	db_storage::mode mode)
{
	throw_if_initialized_at_setup()
		.throw_if_invalid_setup(id, mode);
	
	_id = id;
	_mode = mode;
	
	if (_mode == mode::file_system)
	{
		mkdir("pools", 0777);
	}
	
	return this;
}

db_storage *db_storage::load_db(
	std::string path)
{
	if (access(path.c_str(), F_OK) == -1)
    {
		throw db_storage::invalid_path_exception();
    }
	
	db_storage *db = get_instance();
	
    for (auto const &pool_entry : std::filesystem::directory_iterator(path))
    {
		if (!std::filesystem::is_directory(pool_entry)) continue;
		
		std::string pool_name = pool_entry.path().filename();
		std::string pool_cfg_path = extra_utility::make_path({path, pool_name, "cfg"});
		
		if (access(pool_cfg_path.c_str(), F_OK) == -1) continue;
		
		int b_tree_variant, t_for_b_trees;
		
		std::ifstream stream(pool_cfg_path);
		stream >> b_tree_variant >> t_for_b_trees;
		
		if (stream.fail())
		{
			throw db_storage::load_failure("invalid configs");
		}
		
		add_pool(pool_name, static_cast<search_tree_variant>(b_tree_variant), t_for_b_trees);
		
		for (auto const &schema_entry : std::filesystem::directory_iterator(extra_utility::make_path({path, pool_name})))
		{
			if (!std::filesystem::is_directory(schema_entry)) continue;
			
			std::string schema_name = schema_entry.path().filename();
			std::string schema_cfg_path = extra_utility::make_path({path, pool_name, schema_name, "cfg"});
			
			if (access(schema_cfg_path.c_str(), F_OK) == -1) continue;
			
			int b_tree_variant, t_for_b_trees;
			
			std::ifstream stream(schema_cfg_path);
			stream >> b_tree_variant >> t_for_b_trees;
			
			if (stream.fail())
			{
				throw db_storage::load_failure("invalid configs");
			}
		
			add_schema(pool_name, schema_name, static_cast<search_tree_variant>(b_tree_variant), t_for_b_trees);
			
			for (auto const &collection_entry : std::filesystem::directory_iterator(extra_utility::make_path({path, pool_name, schema_name})))
			{
				if (!std::filesystem::is_directory(collection_entry)) continue;
				
				std::string collection_name = collection_entry.path().filename();
				std::string collection_cfg_path = extra_utility::make_path({path, pool_name, schema_name, collection_name, "cfg"});
			
				if (access(collection_cfg_path.c_str(), F_OK) == -1) continue;
				
				int b_tree_variant, alloc_variant, alloc_fit_mode, t_for_b_trees;
				
				std::ifstream stream(collection_cfg_path);
				stream >> b_tree_variant >> alloc_variant >> alloc_fit_mode >> t_for_b_trees;
				
				if (stream.fail())
				{
					throw db_storage::load_failure("invalid configs");
				}
				
				add_collection(pool_name, schema_name, collection_name,
						static_cast<search_tree_variant>(b_tree_variant),
						static_cast<allocator_variant>(alloc_variant),
						static_cast<allocator_with_fit_mode::fit_mode>(alloc_fit_mode),
						t_for_b_trees);
				
				for (auto const &table_entry : std::filesystem::directory_iterator(extra_utility::make_path({path, pool_name, schema_name, collection_name})))
				{
					if (std::filesystem::is_directory(table_entry)) continue;
					
					if (table_entry.path().filename() == std::to_string(get_instance()->_id))
					{
						load_collection(path, pool_name, schema_name, collection_name);
					}
				}
			}
		}
    }
	
	try
	{
		consolidate();
	}
	catch (std::ios::failure const &ex)
	{
		// okay that was unnessary
	}
	
	return this;
}

db_storage *db_storage::clear()
{
	if (get_instance()->_mode == mode::in_memory_cache)
	{
		return this;
	}
	
	if (access("pools", F_OK) == -1)
    {
		return this;
    }
	
    for (auto const &pool_entry : std::filesystem::directory_iterator("pools"))
    {
		if (!std::filesystem::is_directory(pool_entry)) continue;
		
		std::string pool_name = pool_entry.path().filename();
		std::string pool_cfg_path = extra_utility::make_path({"pools", pool_name, "cfg"});
		
		if (access(pool_cfg_path.c_str(), F_OK) == -1) continue;
		
		for (auto const &schema_entry : std::filesystem::directory_iterator(extra_utility::make_path({"pools", pool_name})))
		{
			if (!std::filesystem::is_directory(schema_entry)) continue;
			
			std::string schema_name = schema_entry.path().filename();
			std::string schema_cfg_path = extra_utility::make_path({"pools", pool_name, schema_name, "cfg"});
			
			if (access(schema_cfg_path.c_str(), F_OK) == -1) continue;
			
			for (auto const &collection_entry : std::filesystem::directory_iterator(extra_utility::make_path({"pools", pool_name, schema_name})))
			{
				if (!std::filesystem::is_directory(collection_entry)) continue;
				
				std::string collection_name = collection_entry.path().filename();
				std::string collection_cfg_path = extra_utility::make_path({"pools", pool_name, schema_name, collection_name, "cfg"});
			
				if (access(collection_cfg_path.c_str(), F_OK) == -1) continue;
				
				for (auto const &table_entry : std::filesystem::directory_iterator(extra_utility::make_path({"pools", pool_name, schema_name, collection_name})))
				{
					if (std::filesystem::is_directory(table_entry)) continue;
					
					if (table_entry.path().filename() == std::to_string(get_instance()->_id))
					{
						remove(table_entry.path().c_str());
					}
				}
			}
		}
    }
	
	return this;
}

db_storage *db_storage::add_pool(
	std::string const &pool_name,
	db_storage::search_tree_variant tree_variant,
	size_t t_for_b_trees)
{
	std::string path = extra_utility::make_path({"pools", pool_name});
	
	throw_if_uninutialized_at_perform()
		.throw_if_invalid_file_name(pool_name)
		.throw_if_path_is_too_long(path)
		.add(pool_name, tree_variant, t_for_b_trees);
	
	if (get_instance()->_mode == mode::file_system)
	{	
		std::string cfg_path = extra_utility::make_path({path, "cfg"});
		
		if (access(path.c_str(), F_OK) == -1 || access(cfg_path.c_str(), F_OK) == -1)
		{
			try
			{
				mkdir(path.c_str(), 0777);
				
				std::ofstream stream(cfg_path);
				if (!stream.is_open())
				{
					throw std::ios::failure("Failed to open the configuration file");
				}
				
				stream << static_cast<int>(tree_variant) << std::endl;
				stream << t_for_b_trees << std::endl;
				stream.flush();
				
				if (stream.fail())
				{
					throw std::ios::failure("Failed to write the configuration file");
				}
			}
			catch (std::ios::failure const &)
			{
				remove(cfg_path.c_str());
				rmdir(path.c_str());
				dispose(path);
				
				throw db_storage::insertion_of_struct_failure();
			}
		}
	}
	
	return this;
}

db_storage *db_storage::dispose_pool(
	std::string const &pool_name)
{
	throw_if_uninutialized_at_perform()
		.dispose(pool_name);
	
	std::string path = extra_utility::make_path({"pools", pool_name});
	// std::string cfg_path = extra_utility::make_path({path, "cfg"});
	
	std::filesystem::remove_all(path);
	
	// remove(cfg_path.c_str());
	// rmdir(path.c_str());
	
	return this;
}

db_storage *db_storage::add_schema(
	std::string const &pool_name,
	std::string const &schema_name,
	db_storage::search_tree_variant tree_variant,
	size_t t_for_b_trees)
{
	std::string old_path = extra_utility::make_path({"pools", pool_name});
	std::string path = extra_utility::make_path({"pools", pool_name, schema_name});
	
	pool &pool = throw_if_uninutialized_at_perform()
					.throw_if_invalid_path(old_path)
					.throw_if_invalid_file_name(schema_name)
					.throw_if_path_is_too_long(path)
					.obtain(pool_name);
	
	pool.add(schema_name, tree_variant, t_for_b_trees);
	
	if (get_instance()->_mode == mode::file_system)
	{
		std::string cfg_path = extra_utility::make_path({path, "cfg"});
		
		if (access(path.c_str(), F_OK) == -1 || access(cfg_path.c_str(), F_OK) == -1)
		{
			try
			{
				mkdir(path.c_str(), 0777);
				
				std::ofstream stream(cfg_path);
				if (!stream.is_open())
				{
					throw std::ios::failure("Failed to create the configuration file");
				}
				
				stream << static_cast<int>(tree_variant) << std::endl;
				stream << t_for_b_trees << std::endl;
				stream.flush();
				
				if (stream.fail())
				{
					throw std::ios::failure("Failed to write the configuration file");
				}
			}
			catch (std::ios::failure const &)
			{
				remove(cfg_path.c_str());
				rmdir(path.c_str());
				dispose(path);
				
				throw db_storage::insertion_of_struct_failure();
			}
		}
	}
	
	return this;
}

db_storage *db_storage::dispose_schema(
	std::string const &pool_name,
	std::string const &schema_name)
{
	throw_if_uninutialized_at_perform()
		.obtain(pool_name)
		.dispose(schema_name);
	
	std::string path = extra_utility::make_path({"pools", pool_name, schema_name});
	// std::string cfg_path = extra_utility::make_path({path, "cfg"});
	
	std::filesystem::remove_all(path);
	
	// remove(cfg_path.c_str());
	// rmdir(path.c_str());
	
	return this;
}

db_storage *db_storage::add_collection(
	std::string const &pool_name,
	std::string const &schema_name,
	std::string const &collection_name,
	db_storage::search_tree_variant tree_variant,
	db_storage::allocator_variant allocator_variant,
	allocator_with_fit_mode::fit_mode fit_mode,
	size_t t_for_b_trees)
{
	std::string old_path = extra_utility::make_path({"pools", pool_name, schema_name});
	std::string path = extra_utility::make_path({"pools", pool_name, schema_name, collection_name});
	
	schema &schema = throw_if_uninutialized_at_perform()
						.throw_if_invalid_path(old_path)
						.throw_if_invalid_file_name(collection_name)
						.throw_if_path_is_too_long(path)
						.obtain(pool_name)
						.obtain(schema_name);
	
	schema.add(collection_name, tree_variant, allocator_variant, fit_mode, t_for_b_trees);
	
	if (get_instance()->_mode == mode::file_system)
	{
		std::string cfg_path = extra_utility::make_path({path, "cfg"});
		std::string strg_path = extra_utility::make_path({path, std::to_string(_id)});
		
		if (access(path.c_str(), F_OK) == -1 || access(cfg_path.c_str(), F_OK) == -1 ||
				access(strg_path.c_str(), F_OK) == -1)
		{
			try
			{
				mkdir(path.c_str(), 0777);
				
				std::ofstream strg_stream(strg_path.c_str());
				strg_stream.close();
				
				if (access(strg_path.c_str(), F_OK) == -1)
				{
					throw std::ios::failure("Failed to create the storage file");
				}
				
				std::ofstream stream(cfg_path);
				if (!stream.is_open())
				{
					throw std::ios::failure("Failed to open the configuration file");
				}
				
				stream << static_cast<int>(tree_variant) << std::endl;
				stream << static_cast<int>(allocator_variant) << std::endl;
				stream << static_cast<int>(fit_mode) << std::endl;
				stream << t_for_b_trees << std::endl;
				stream.flush();
				
				if (stream.fail())
				{
					throw std::ios::failure("Failed to write the configuration file");
				}
			}
			catch (std::ios::failure const &)
			{
				remove(strg_path.c_str());
				remove(cfg_path.c_str());
				rmdir(path.c_str());
				schema.dispose(collection_name);
				
				throw db_storage::insertion_of_struct_failure();
			}
		}
	}
	
	return this;
}

db_storage *db_storage::dispose_collection(
	std::string const &pool_name,
	std::string const &schema_name,
	std::string const &collection_name)
{
	throw_if_uninutialized_at_perform()
		.obtain(pool_name)
		.obtain(schema_name)
		.dispose(collection_name);
	
	std::string path = extra_utility::make_path({"pools", pool_name, schema_name, collection_name});
	// std::string tmp_path = extra_utility::make_path({path, "tmp"});
	// std::string strg_path = extra_utility::make_path({path, std::to_string(_id)});
	// std::string cfg_path = extra_utility::make_path({path, "cfg"});
	
	std::filesystem::remove_all(path);
	
	// remove(tmp_path.c_str());
	// remove(strg_path.c_str());
	// remove(cfg_path.c_str());
	// rmdir(path.c_str());
	
	return this;
}

db_storage *db_storage::add(
	std::string const &pool_name,
	std::string const &schema_name,
	std::string const &collection_name,
	tkey const &key,
	tvalue const &value)
{
	std::string path = extra_utility::make_path({"pools", pool_name, schema_name, collection_name, std::to_string(_id)});
	
	throw_if_uninutialized_at_perform()
		.throw_if_invalid_path(path)
		.obtain(pool_name)
		.obtain(schema_name)
		.obtain(collection_name)
		.insert(key, value, path);
	
	return this;
}

db_storage *db_storage::add(
	std::string const &pool_name,
	std::string const &schema_name,
	std::string const &collection_name,
	tkey const &key,
	tvalue &&value)
{
	std::string path = extra_utility::make_path({"pools", pool_name, schema_name, collection_name, std::to_string(_id)});
	
	throw_if_uninutialized_at_perform()
		.throw_if_invalid_path(path)
		.obtain(pool_name)
		.obtain(schema_name)
		.obtain(collection_name)
		.insert(key, std::move(value), path);
	
	return this;
}

db_storage *db_storage::update(
	std::string const &pool_name,
	std::string const &schema_name,
	std::string const &collection_name,
	tkey const &key,
	tvalue const &value)
{
	std::string path = extra_utility::make_path({"pools", pool_name, schema_name, collection_name, std::to_string(_id)});
	
	throw_if_uninutialized_at_perform()
		.throw_if_invalid_path(path)
		.obtain(pool_name)
		.obtain(schema_name)
		.obtain(collection_name)
		.update(key, value, path);
	
	return this;
}

db_storage *db_storage::update(
	std::string const &pool_name,
	std::string const &schema_name,
	std::string const &collection_name,
	tkey const &key,
	tvalue &&value)
{
	std::string path = extra_utility::make_path({"pools", pool_name, schema_name, collection_name, std::to_string(_id)});
	
	throw_if_uninutialized_at_perform()
		.throw_if_invalid_path(path)
		.obtain(pool_name)
		.obtain(schema_name)
		.obtain(collection_name)
		.update(key, std::move(value), path);
	
	return this;
}

tvalue db_storage::dispose(
	std::string const &pool_name,
	std::string const &schema_name,
	std::string const &collection_name,
	tkey const &key)
{
	std::string path = extra_utility::make_path({"pools", pool_name, schema_name, collection_name, std::to_string(_id)});
	
	return throw_if_uninutialized_at_perform()
				.throw_if_invalid_path(path)
				.obtain(pool_name)
				.obtain(schema_name)
				.obtain(collection_name)
				.dispose(key, path);
}

tvalue db_storage::obtain(
	std::string const &pool_name,
	std::string const &schema_name,
	std::string const &collection_name,
	tkey const &key)
{
	std::string path = extra_utility::make_path({"pools", pool_name, schema_name, collection_name, std::to_string(_id)});
	
	return throw_if_uninutialized_at_perform()
			.throw_if_invalid_path(path)
			.obtain(pool_name)
			.obtain(schema_name)
			.obtain(collection_name)
			.obtain(key, path);
}

std::vector<std::pair<tkey, tvalue>> db_storage::obtain_between(
	std::string const &pool_name,
	std::string const &schema_name,
	std::string const &collection_name,
	tkey const &lower_bound,
	tkey const &upper_bound,
	bool lower_bound_inclusive,
	bool upper_bound_inclusive)
{
	std::string path = extra_utility::make_path({"pools", pool_name, schema_name, collection_name, std::to_string(_id)});
	
	return throw_if_uninutialized_at_perform()
			.throw_if_invalid_path(path)
			.obtain(pool_name)
			.obtain(schema_name)
			.obtain(collection_name)
			.obtain_between(lower_bound, upper_bound, lower_bound_inclusive, upper_bound_inclusive, path);
}

std::pair<tkey, tvalue> db_storage::obtain_min(
	std::string const &pool_name,
	std::string const &schema_name,
	std::string const &collection_name)
{
	std::string path = extra_utility::make_path({"pools", pool_name, schema_name, collection_name, std::to_string(_id)});
	
	return throw_if_uninutialized_at_perform()
			.throw_if_invalid_path(path)
			.obtain(pool_name)
			.obtain(schema_name)
			.obtain(collection_name)
			.obtain_min(path);
}

std::pair<tkey, tvalue> db_storage::obtain_max(
	std::string const &pool_name,
	std::string const &schema_name,
	std::string const &collection_name)
{
	std::string path = extra_utility::make_path({"pools", pool_name, schema_name, collection_name, std::to_string(_id)});
	
	return throw_if_uninutialized_at_perform()
			.throw_if_invalid_path(path)
			.obtain(pool_name)
			.obtain(schema_name)
			.obtain(collection_name)
			.obtain_max(path);
}

std::pair<tkey, tvalue> db_storage::obtain_next(
	std::string const &pool_name,
	std::string const &schema_name,
	std::string const &collection_name,
	tkey const &key)
{
	std::string path = extra_utility::make_path({"pools", pool_name, schema_name, collection_name, std::to_string(_id)});
	
	return throw_if_uninutialized_at_perform()
			.throw_if_invalid_path(path)
			.obtain(pool_name)
			.obtain(schema_name)
			.obtain(collection_name)
			.obtain_next(path, key);
}

db_storage *db_storage::consolidate()
{
	auto iter = _pools.begin_infix();
	auto iter_end = _pools.end_infix();
	
	for (; iter != iter_end; ++iter)
	{
		std::get<3>(*iter).consolidate(extra_utility::make_path({"pools", std::get<2>(*iter)}));
	}
	
	return this;
}

size_t db_storage::get_collection_records_cnt(
	std::string const &pool_name,
	std::string const &schema_name,
	std::string const &collection_name)
{
	std::string path = extra_utility::make_path({"pools", pool_name, schema_name, collection_name, std::to_string(_id)});
	
	return throw_if_uninutialized_at_perform()
			.throw_if_invalid_path(path)
			.obtain(pool_name)
			.obtain(schema_name)
			.obtain(collection_name)
			.get_records_cnt();
}

#pragma endregion db storage public operations implementation

#pragma region db storage utility data operations implementation

void db_storage::add(
	std::string const &pool_name,
	search_tree_variant tree_variant,
	size_t t_for_b_trees)
{
	try
	{
		_pools.insert(pool_name, pool(tree_variant, t_for_b_trees));		
	}
	catch (search_tree<std::string, pool>::insertion_of_existent_key_attempt_exception_exception const &)
	{
		throw db_storage::insertion_of_existent_struct_attempt_exception();
		// TODO;
	}
}

void db_storage::dispose(
	std::string const &pool_name)
{
	try
	{
		_pools.dispose(pool_name);
	}
	catch (search_tree<std::string, pool>::disposal_of_nonexistent_key_attempt_exception const &)
	{
		throw db_storage::disposal_of_nonexistent_struct_attempt_exception();
		// TODO;
	}
}

db_storage::pool &db_storage::obtain(
	std::string const &pool_name)
{
	try
	{
		return _pools.obtain(pool_name);
	}
	catch (search_tree<std::string, pool>::obtaining_of_nonexistent_key_attempt_exception const &)
	{
		throw db_storage::invalid_path_exception();
		// TODO;
	}
}

void db_storage::load_collection(
	std::string prefix,
	std::string pool_name,
	std::string schema_name,
	std::string collection_name)
{
	std::string data_path = extra_utility::make_path({prefix, pool_name, schema_name, collection_name, std::to_string(get_instance()->_id)});
	
	std::ifstream data_stream(data_path, std::ios::binary);
    if (!data_stream.is_open())
    {
        throw std::ios::failure("Failed to load collection");
    }
	
	long file_pos = 0;
	
    while (data_stream.peek() != EOF)
    {
		char ch;
		tkey login;
		tvalue value;
		size_t login_len, name_len;
		
		data_stream.read(reinterpret_cast<char *>(&login_len), sizeof(size_t));



		value.name->get_data().reserve(login_len);
		for (size_t i = 0; i < login_len; ++i)
		{
			data_stream.read(&ch, sizeof(char));
			login->get_data().push_back(ch);
		}
		
		data_stream.read(reinterpret_cast<char *>(&value.personal_id), sizeof(size_t));
		data_stream.read(reinterpret_cast<char *>(&name_len), sizeof(size_t));
		
		value.name->get_data().reserve(name_len);
		for (size_t i = 0; i < name_len; ++i)
		{
			data_stream.read(&ch, sizeof(char));
			value.name->get_data().push_back(ch);
		}
		
		get_instance()->obtain(pool_name)
						.obtain(schema_name)
						.obtain(collection_name)
						.load(login, std::move(value), data_path, file_pos);
		
		file_pos = data_stream.tellg();
    }
	
	if (data_stream.fail())
    {
        throw std::ios::failure("Failed to load collection");
    }
}

#pragma endregion db storage utility data operations implementation

#pragma region db storage utility common operations

// no common operations?

#pragma endregion db storage utility common operations

#pragma region db storage validators implementation

db_storage &db_storage::throw_if_initialized_at_setup()
{
	if (_mode == mode::uninitialized)
	{
		return *this;
	}
	
	throw db_storage::setup_failure("attempt to change previously set up mode");
}

db_storage &db_storage::throw_if_invalid_setup(
	size_t id,
	db_storage::mode mode)
{
	if (mode != mode::uninitialized && id > 0)
	{
		return *this;
	}
	
	throw db_storage::setup_failure("invalid setup data");
}

db_storage &db_storage::throw_if_uninutialized_at_perform()
{
	if (_mode != mode::uninitialized)
	{
		return *this;
	}
	
	throw std::logic_error("attempt to perform an operation while mode not initialized"); // TODO CHANGE?
}

db_storage &db_storage::throw_if_invalid_path(
	std::string const &path)
{
	if (_mode == mode::file_system)
	{
		if (access(path.c_str(), F_OK) == -1)
		{
			throw db_storage::invalid_path_exception();
		}
	}
	
	return *this;
}

db_storage &db_storage::throw_if_invalid_file_name(
	std::string const &file_name)
{
	if (_mode == mode::file_system)
	{
		bool space = false;
		for (char ch : file_name)
		{
			if (!isalnum(ch) && ch != '_' && ch != '-' && ch != ' ')
			{
				throw db_storage::invalid_struct_name_exception();
			}
			space = ch == ' ';
		}
		if (space)
		{
			throw db_storage::invalid_struct_name_exception();
		}
	}

	return *this;
}

db_storage &db_storage::throw_if_path_is_too_long(
		std::string const &path)
{
	if (_mode == mode::file_system)
	{
		char cwd[PATH_MAX];
		char *code = getcwd(cwd, PATH_MAX);
		if (code == nullptr)
		{
			throw db_storage::invalid_path_exception();
		}
		if ((std::string(cwd) + path).size() > PATH_MAX - 10)
		{
			throw db_storage::too_big_path_exception();
		}
	}

	return *this;
}

#pragma endregion db storage validators implementation
