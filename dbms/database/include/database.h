

class db_server final
{

public:

    enum class mode
    {
        uninitialized,
        in_memory_cache,
        file_system
    };

    enum class search_tree_variant
    {
        AVL,
        red_black,
        splay,
        scapegoat,
        b,
        b_plus,
        b_star,
        b_star_plus
    };

private:

    class chain_of_responsibility;

    class collection final
    {

    private:

        search_tree<std::string, search_tree<tdata *, std::vector<chain_of_responsibility>>> *_data;
        search_tree_variant _variant;

    public:

        explicit collection(
            search_tree_variant variant,
            size_t t_for_b_trees = 8):
                _variant(variant)
        {
            switch (variant)
            {
                case search_tree_variant::AVL:
                    // _collections = new AVL_tree<std::string, schema>();
                    // break;
                case search_tree_variant::red_black:
                    // _collections = new red_black_tree<std::string, schema>();
                    // break;
                case search_tree_variant::splay:
                    // _collections = new splay_tree<std::string, schema>();
                    // break;
                case search_tree_variant::scapegoat:
                    // _collections = new scapegoat_tree<std::string, schema>();
                    // break;
                case search_tree_variant::b_plus:
                    // _collections = new b_plus<std::string, schema>();
                    // break;
                case search_tree_variant::b_star:
                    // _collections = new b_star_tree<std::string, schema>();
                    // break;
                case search_tree_variant::b_star_plus:
                    // _collections = new b_star_plus_tree<std::string, schema>();
                    // break;
                case search_tree_variant::b:
                    _data = new b_tree<tkey, tvalue>(t_for_b_trees, tkey_comparer());
                    break;
            }
        }

    private:

        void clear()
        {
            delete _data;
            _data = nullptr;
        }

        void copy_from(
            collection const &other)
        {
            switch (_variant = other._variant)
            {
                case search_tree_variant::AVL:
                    // _schemas = new AVL_tree<std::string, schema>();
                    // break;
                case search_tree_variant::red_black:
                    // _schemas = new red_black_tree<std::string, schema>();
                    // break;
                case search_tree_variant::splay:
                    // _schemas = new splay_tree<std::string, schema>();
                    // break;
                case search_tree_variant::scapegoat:
                    // _schemas = new scapegoat_tree<std::string, schema>();
                    // break;
                case search_tree_variant::b_plus:
                    // _schemas = new b_plus<std::string, schema>();
                    // break;
                case search_tree_variant::b_star:
                    // _schemas = new b_star_tree<std::string, schema>();
                    // break;
                case search_tree_variant::b_star_plus:
                    // _schemas = new b_star_plus_tree<std::string, schema>();
                    // break;
                case search_tree_variant::b:
                    _data = new b_tree<tkey, tvalue>(
                        *dynamic_cast<b_tree<tkey, tvalue> *>(other._data));
                    break;
            }
        }

        void move_from(
            collection &&other)
        {
            switch (_variant = other._variant)
            {
                case search_tree_variant::AVL:
                    // _schemas = new AVL_tree<std::string, schema>();
                    // break;
                case search_tree_variant::red_black:
                    // _schemas = new red_black_tree<std::string, schema>();
                    // break;
                case search_tree_variant::splay:
                    // _schemas = new splay_tree<std::string, schema>();
                    // break;
                case search_tree_variant::scapegoat:
                    // _schemas = new scapegoat_tree<std::string, schema>();
                    // break;
                case search_tree_variant::b_plus:
                    // _schemas = new b_plus<std::string, schema>();
                    // break;
                case search_tree_variant::b_star:
                    // _schemas = new b_star_tree<std::string, schema>();
                    // break;
                case search_tree_variant::b_star_plus:
                    // _schemas = new b_star_plus_tree<std::string, schema>();
                    // break;
                case search_tree_variant::b:
                    _data = new b_tree<tkey, tvalue>(
                        std::move(*dynamic_cast<b_tree<tkey, tvalue> *>(other._data)));
                    break;
            }
        }

    public:

        ~collection()
        {
            clear();
        }

        collection(
            collection const &other)
        {
            copy_from(other);
        }

        collection &operator=(
            collection const &other)
        {
            if (this != &other)
            {
                clear();
                copy_from(other);
            }

            return *this;
        }

        collection(
            collection &&other) noexcept
        {
            move_from(std::move(other));
        }

        collection &operator=(
            collection &&other) noexcept
        {
            if (this != &other)
            {
                clear();
                move_from(std::move(other));
            }

            return *this;
        }

    public:

        void insert(
            tkey const &key,
            tvalue const &value)
        {
            _data->insert(key, value);
        }

        void insert(
            tkey const &key,
            tvalue &&value)
        {
            _data->insert(key, std::move(value));
        }

        void update(
            tkey const &key,
            tvalue const &value)
        {
            _data->update(key, value);
        }

        void update(
            tkey const &key,
            tvalue &&value)
        {
            _data->update(key, std::move(value));
        }

        tvalue &obtain(
            tkey const &key)
        {
            return _data->obtain(key);
        }

        tvalue dispose(
            tkey const &key)
        {
            return _data->dispose(key);
        }

        std::vector<typename associative_container<tkey, tvalue>::key_value_pair> obtain_between(
            tkey const &lower_bound,
            tkey const &upper_bound,
            bool lower_bound_inclusive,
            bool upper_bound_inclusive)
        {
            return _data->obtain_between(lower_bound, upper_bound, lower_bound_inclusive, upper_bound_inclusive);
        }

    };

    class schema final
    {

    private:

        search_tree<std::string, collection> *_collections;
        search_tree_variant _variant;

    public:

        explicit schema(
            search_tree_variant variant,
            size_t t_for_b_trees = 8):
                _variant(variant)
        {
            switch (variant)
            {
                case search_tree_variant::AVL:
                    // _collections = new AVL_tree<std::string, schema>();
                    // break;
                case search_tree_variant::red_black:
                    // _collections = new red_black_tree<std::string, schema>();
                    // break;
                case search_tree_variant::splay:
                    // _collections = new splay_tree<std::string, schema>();
                    // break;
                case search_tree_variant::scapegoat:
                    // _collections = new scapegoat_tree<std::string, schema>();
                    // break;
                case search_tree_variant::b_plus:
                    // _collections = new b_plus<std::string, schema>();
                    // break;
                case search_tree_variant::b_star:
                    // _collections = new b_star_tree<std::string, schema>();
                    // break;
                case search_tree_variant::b_star_plus:
                    // _collections = new b_star_plus_tree<std::string, schema>();
                    // break;
                case search_tree_variant::b:
                    _collections = new b_tree<std::string, collection>(t_for_b_trees, stdstring_comparer());
                    break;
            }
        }

    private:

        void clear()
        {
            delete _collections;
            _collections = nullptr;
        }

        void copy_from(
            schema const &other)
        {
            switch (_variant = other._variant)
            {
                case search_tree_variant::AVL:
                    // _schemas = new AVL_tree<std::string, schema>();
                    // break;
                case search_tree_variant::red_black:
                    // _schemas = new red_black_tree<std::string, schema>();
                    // break;
                case search_tree_variant::splay:
                    // _schemas = new splay_tree<std::string, schema>();
                    // break;
                case search_tree_variant::scapegoat:
                    // _schemas = new scapegoat_tree<std::string, schema>();
                    // break;
                case search_tree_variant::b_plus:
                    // _schemas = new b_plus<std::string, schema>();
                    // break;
                case search_tree_variant::b_star:
                    // _schemas = new b_star_tree<std::string, schema>();
                    // break;
                case search_tree_variant::b_star_plus:
                    // _schemas = new b_star_plus_tree<std::string, schema>();
                    // break;
                case search_tree_variant::b:
                    _collections = new b_tree<std::string, collection>(*dynamic_cast<b_tree<std::string, collection> *>(other._collections));
                    break;
            }
        }

        void move_from(
            schema &&other)
        {
            switch (_variant = other._variant)
            {
                case search_tree_variant::AVL:
                    // _schemas = new AVL_tree<std::string, schema>();
                    // break;
                case search_tree_variant::red_black:
                    // _schemas = new red_black_tree<std::string, schema>();
                    // break;
                case search_tree_variant::splay:
                    // _schemas = new splay_tree<std::string, schema>();
                    // break;
                case search_tree_variant::scapegoat:
                    // _schemas = new scapegoat_tree<std::string, schema>();
                    // break;
                case search_tree_variant::b_plus:
                    // _schemas = new b_plus<std::string, schema>();
                    // break;
                case search_tree_variant::b_star:
                    // _schemas = new b_star_tree<std::string, schema>();
                    // break;
                case search_tree_variant::b_star_plus:
                    // _schemas = new b_star_plus_tree<std::string, schema>();
                    // break;
                case search_tree_variant::b:
                    _collections = new b_tree<std::string, collection>(std::move(*dynamic_cast<b_tree<std::string, collection> *>(other._collections)));
                    break;
            }
        }

    public:

        ~schema()
        {
            clear();
        }

        schema(
            schema const &other)
        {
            copy_from(other);
        }

        schema &operator=(
            schema const &other)
        {
            if (this != &other)
            {
                clear();
                copy_from(other);
            }

            return *this;
        }

        schema(
            schema &&other) noexcept
        {
            move_from(std::move(other));
        }

        schema &operator=(
            schema &&other) noexcept
        {
            if (this != &other)
            {
                clear();
                move_from(std::move(other));
            }

            return *this;
        }

    public:

        void add(
            std::string const &collection_name,
            search_tree_variant variant,
            size_t t_for_b_trees = 8)
        {
            _collections->insert(collection_name, collection(variant, t_for_b_trees));
        }

        collection &obtain(
            std::string const &collection_name)
        {
            return _collections->obtain(collection_name);
        }

        void dispose(
            std::string const &collection_name)
        {
            _collections->dispose(collection_name);
        }

    };

    class pool final
    {

    private:

        search_tree<std::string, schema> *_schemas;
        search_tree_variant _variant;

    public:

        explicit pool(
            search_tree_variant variant,
            size_t t_for_b_trees = 8):
                _variant(variant)
        {
            switch (variant)
            {
                case search_tree_variant::AVL:
                    // _schemas = new AVL_tree<std::string, schema>();
                    // break;
                case search_tree_variant::red_black:
                    // _schemas = new red_black_tree<std::string, schema>();
                    // break;
                case search_tree_variant::splay:
                    // _schemas = new splay_tree<std::string, schema>();
                    // break;
                case search_tree_variant::scapegoat:
                    // _schemas = new scapegoat_tree<std::string, schema>();
                    // break;
                case search_tree_variant::b_plus:
                    // _schemas = new b_plus<std::string, schema>();
                    // break;
                case search_tree_variant::b_star:
                    // _schemas = new b_star_tree<std::string, schema>();
                    // break;
                case search_tree_variant::b_star_plus:
                    // _schemas = new b_star_plus_tree<std::string, schema>();
                    // break;
                case search_tree_variant::b:
                    _schemas = new b_tree<std::string, schema>(t_for_b_trees, stdstring_comparer());
                    break;
            }
        }

    private:

        void clear()
        {
            delete _schemas;
            _schemas = nullptr;
        }

        void copy_from(
            pool const &other)
        {
            switch (_variant = other._variant)
            {
                case search_tree_variant::AVL:
                    // _schemas = new AVL_tree<std::string, schema>();
                    // break;
                case search_tree_variant::red_black:
                    // _schemas = new red_black_tree<std::string, schema>();
                    // break;
                case search_tree_variant::splay:
                    // _schemas = new splay_tree<std::string, schema>();
                    // break;
                case search_tree_variant::scapegoat:
                    // _schemas = new scapegoat_tree<std::string, schema>();
                    // break;
                case search_tree_variant::b_plus:
                    // _schemas = new b_plus<std::string, schema>();
                    // break;
                case search_tree_variant::b_star:
                    // _schemas = new b_star_tree<std::string, schema>();
                    // break;
                case search_tree_variant::b_star_plus:
                    // _schemas = new b_star_plus_tree<std::string, schema>();
                    // break;
                case search_tree_variant::b:
                    _schemas = new b_tree<std::string, schema>(*dynamic_cast<b_tree<std::string, schema> *>(other._schemas));
                    break;
            }
        }

        void move_from(
            pool &&other)
        {
            switch (_variant = other._variant)
            {
                case search_tree_variant::AVL:
                    // _schemas = new AVL_tree<std::string, schema>();
                    // break;
                case search_tree_variant::red_black:
                    // _schemas = new red_black_tree<std::string, schema>();
                    // break;
                case search_tree_variant::splay:
                    // _schemas = new splay_tree<std::string, schema>();
                    // break;
                case search_tree_variant::scapegoat:
                    // _schemas = new scapegoat_tree<std::string, schema>();
                    // break;
                case search_tree_variant::b_plus:
                    // _schemas = new b_plus<std::string, schema>();
                    // break;
                case search_tree_variant::b_star:
                    // _schemas = new b_star_tree<std::string, schema>();
                    // break;
                case search_tree_variant::b_star_plus:
                    // _schemas = new b_star_plus_tree<std::string, schema>();
                    // break;
                case search_tree_variant::b:
                    _schemas = new b_tree<std::string, schema>(std::move(*dynamic_cast<b_tree<std::string, schema> *>(other._schemas)));
                    break;
            }
        }

    public:

        ~pool()
        {
            clear();
        }

        pool(
            pool const &other)
        {
            copy_from(other);
        }

        pool &operator=(
            pool const &other)
        {
            if (this != &other)
            {
                clear();
                copy_from(other);
            }

            return *this;
        }

        pool(
            pool &&other) noexcept
        {
            move_from(std::move(other));
        }

        pool &operator=(
            pool &&other) noexcept
        {
            if (this != &other)
            {
                clear();
                move_from(std::move(other));
            }

            return *this;
        }

    public:

        void add(
            std::string const &schema_name,
            search_tree_variant variant,
            size_t t_for_b_trees = 8)
        {
            _schemas->insert(schema_name, schema(variant, t_for_b_trees));
        }

        schema &obtain(
            std::string const &schema_name)
        {
            return _schemas->obtain(schema_name);
        }

        void dispose(
            std::string const &schema_name)
        {
            _schemas->dispose(schema_name);
        }

    };

private:

    class command
    {

    public:

        virtual ~command() noexcept = default;

    public:

        virtual void execute(
            bool &data_exists,
            std::optional<tdata> &data_to_modify) const = 0;

    };

    class insert_command final:
        public command
    {

    private:

        tdata _initial_version;

    public:

        explicit insert_command(
            tdata &&initial_version):
                _initial_version(initial_version)
        {

        }

    public:

        void execute(
            bool &data_exists,
            std::optional<tdata> &data_to_modify) const override
        {
            if (data_exists)
            {
                throw std::logic_error("attempt to insert already existent data");
            }

            if (data_to_modify.has_value())
            {
                data_to_modify.value() = _initial_version;
            }

            data_exists = true;
        }

    };

    class update_command final:
        public command
    {

    private:

        std::string _update_expression;

    public:

        explicit update_command(
            std::string const &update_expression):
                _update_expression(update_expression)
        {

        }

    public:

        void execute(
            bool &data_exists,
            std::optional<tdata> &data_to_modify) const override
        {
            if (!data_exists)
            {
                throw std::logic_error("attempt to modify non-existent data");
            }

            if (data_to_modify.has_value())
            {
                // TODO: interpreter for update expression
            }
        }

    };

    class dispose_command final:
        public command
    {

    public:

        void execute(
            bool &data_exists,
            std::optional<tdata> &data_to_modify) const override
        {
            if (!data_exists)
            {
                throw std::logic_error("attempt to dispose non-existent data");
            }

            data_exists = false;
        }

    };

private:

    class chain_of_responsibility_handler final
    {

        friend class chain_of_responsibility;

    private:

        command *_command;
        std::int64_t _date_time_activity_started;
        chain_of_responsibility_handler *_next_handler;

    public:

        chain_of_responsibility_handler(
            command *command,
            std::int64_t date_time_activity_started):
                _command(command),
                _date_time_activity_started(date_time_activity_started),
                _next_handler(nullptr)
        {

        }

    public:

        void handle(
            bool &data_exists,
            std::optional<tdata> &data_to_modify,
            std::int64_t date_time_target) const
        {
            if (date_time_target <= _date_time_activity_started)
            {
                return;
            }

            _command->execute(data_exists, data_to_modify);

            if (_next_handler != nullptr)
            {
                _next_handler->handle(data_exists, data_to_modify, date_time_target);
            }
        }

    };

    class chain_of_responsibility final
    {

    private:

        chain_of_responsibility_handler *_first_handler;
        chain_of_responsibility_handler *_last_handler;

    public:

        chain_of_responsibility():
            _first_handler(nullptr),
            _last_handler(nullptr)
        {

        }

    private:

        void add_handler(
            command *command)
        {
            // TODO: obtain date/time from OS
            // TODO: convert date/time from OS to std::int64_t
            std::int64_t date_time_activity_started = 0;

            auto *added_handler = new chain_of_responsibility_handler(command, date_time_activity_started);

            if (_last_handler == nullptr)
            {
                _first_handler = _last_handler = added_handler;
            }
            else
            {
                _last_handler->_next_handler = added_handler;
                _last_handler = added_handler;
            }
        }

        void handle(
            bool &data_exists,
            std::optional<tdata> &data_to_modify,
            std::int64_t date_time_target)
        {
            if (_first_handler == nullptr)
            {
                return;
            }

            _first_handler->handle(data_exists, data_to_modify, date_time_target);
        }

    public:

        void insert(
            tdata &&data_to_insert)
        {
            bool data_exists = false;
            std::optional<tdata> empty_optional;
            // TODO: obtain date/time from OS
            // TODO: convert date/time from OS to std::int64_t
            std::int64_t current_date_time = 0;
            handle(data_exists, empty_optional, current_date_time);

            if (data_exists)
            {
                throw std::logic_error("attempt to add data insertion handler while data exists");
            }

            add_handler(new insert_command(std::move(data_to_insert)));
        }

        std::optional<tdata> obtain(
            std::int64_t date_time_of_activity)
        {
            bool data_exists;
            tdata data_to_modify;
            auto data_to_modify_optional = std::make_optional(data_to_modify);

            handle(data_exists, data_to_modify_optional, date_time_of_activity);

            return data_exists
                ? std::make_optional(std::move(data_to_modify))
                : std::optional<tdata>();
        }

        void update(
            tdata const &data,
            std::string const &update_expression)
        {
            bool data_exists = false;
            std::optional<tdata> empty_optional;
            // TODO: obtain date/time from OS
            // TODO: convert date/time from OS to std::int64_t
            std::int64_t current_date_time = 0;
            handle(data_exists, empty_optional, current_date_time);

            if (!data_exists)
            {
                throw std::logic_error("attempt to add data update handler while data not exists");
            }

            add_handler(new update_command(update_expression));
        }

        void dispose(
            tdata const &data)
        {
            bool data_exists = false;
            std::optional<tdata> empty_optional;
            // TODO: obtain date/time from OS
            // TODO: convert date/time from OS to std::int64_t
            std::int64_t current_date_time = 0;
            handle(data_exists, empty_optional, current_date_time);

            if (!data_exists)
            {
                throw std::logic_error("attempt to add data dispose handler while data not exists");
            }

            add_handler(new dispose_command);
        }

    };

private:

    // static db_server *_instance;
    // static std::mutex _sync_object;
    b_tree<std::string, pool> _pools;
    mode _mode;

public:

    static db_server *get_instance()
    {
        // if (_instance == nullptr)
        // {
        //     std::lock_guard<std::mutex> lock(_sync_object);
        //     if (_instance == nullptr)
        //     {
        //         _instance = new db_server();
        //     }
        // }
        //
        // return _instance;

        static auto *instance = new db_server();
        return instance;
    }

private:

    db_server():
        _pools(8, stdstring_comparer()),
        _mode(mode::uninitialized)
    {

    }

public:

    db_server(
        db_server const &) = delete;

    db_server(
        db_server &&) = delete;

private:

    void add(
        std::string const &pool_name,
        search_tree_variant variant,
        size_t t_for_b_trees = 8)
    {
        _pools.insert(pool_name, pool(variant, t_for_b_trees));
    }

    pool &obtain(
        std::string const &pool_name)
    {
        if (_mode == mode::file_system)
        {
            // fseek
            // TODO: check existence of path
        }
        return _pools.obtain(pool_name);
    }

    void dispose(
        std::string const &pool_name)
    {
        _pools.dispose(pool_name);
    }

public:

    db_server *add_pool(
        std::string const &pool_name,
        search_tree_variant variant,
        size_t t_for_b_trees = 8)
    {
        throw_if_uninitialized_at_perform()
            .add(pool_name, variant, t_for_b_trees);

        return this;
    }

    db_server *dispose_pool(
        std::string const &pool_name)
    {
        throw_if_uninitialized_at_perform()
            .dispose(pool_name);

        return this;
    }

    db_server *add_schema(
        std::string const &pool_name,
        std::string const &schema_name,
        search_tree_variant variant,
        size_t t_for_b_trees = 8)
    {
        throw_if_uninitialized_at_perform()
            .obtain(pool_name)
            .add(schema_name, variant, t_for_b_trees);

        return this;
    }

    db_server *dispose_schema(
        std::string const &pool_name,
        std::string const &schema_name)
    {
        throw_if_uninitialized_at_perform()
            .obtain(pool_name)
            .dispose(schema_name);

        return this;
    }

    db_server *add_collection(
        std::string const &pool_name,
        std::string const &schema_name,
        std::string const &collection_name,
        search_tree_variant variant,
        size_t t_for_b_trees = 8)
    {
        throw_if_uninitialized_at_perform()
            .throw_if_invalid_path(pool_name)
            .throw_if_invalid_path(schema_name)
            .throw_if_invalid_file_name(collection_name)
            .throw_if_path_is_too_long(pool_name, schema_name, collection_name)
            .obtain(pool_name)
            .obtain(schema_name)
            .add(collection_name, variant, t_for_b_trees);

        return this;
    }

    db_server *dispose_collection(
        std::string const &pool_name,
        std::string const &schema_name,
        std::string const &collection_name)
    {
        throw_if_uninitialized_at_perform()
            .obtain(pool_name)
            .obtain(schema_name)
            .dispose(collection_name);

        return this;
    }

    db_server *add(
        std::string const &pool_name,
        std::string const &schema_name,
        std::string const &collection_name,
        tkey const &key,
        tvalue const &value)
    {
        throw_if_uninitialized_at_perform()
            .obtain(pool_name)
            .obtain(schema_name)
            .obtain(collection_name)
            .insert(key, value);

        return this;
    }

    db_server *add(
        std::string const &pool_name,
        std::string const &schema_name,
        std::string const &collection_name,
        tkey const &key,
        tvalue &&value)
    {
        throw_if_uninitialized_at_perform()
            .obtain(pool_name)
            .obtain(schema_name)
            .obtain(collection_name)
            .insert(key, std::move(value));

        return this;
    }

    db_server *update(
        std::string const &pool_name,
        std::string const &schema_name,
        std::string const &collection_name,
        tkey const &key,
        tvalue const &value)
    {
        throw_if_uninitialized_at_perform()
            .obtain(pool_name)
            .obtain(schema_name)
            .obtain(collection_name)
            .update(key, value);

        return this;
    }

    db_server *update(
        std::string const &pool_name,
        std::string const &schema_name,
        std::string const &collection_name,
        tkey const &key,
        tvalue &&value)
    {
        throw_if_uninitialized_at_perform()
            .obtain(pool_name)
            .obtain(schema_name)
            .obtain(collection_name)
            .update(key, std::move(value));

        return this;
    }

    tvalue &obtain(
        std::string const &pool_name,
        std::string const &schema_name,
        std::string const &collection_name,
        tkey const &key)
    {
        return throw_if_uninitialized_at_perform()
            .obtain(pool_name)
            .obtain(schema_name)
            .obtain(collection_name)
            .obtain(key);
    }

    std::vector<typename associative_container<tkey, tvalue>::key_value_pair> obtain_between(
        std::string const &pool_name,
        std::string const &schema_name,
        std::string const &collection_name,
        tkey const &lower_bound,
        tkey const &upper_bound,
        bool lower_bound_inclusive,
        bool upper_bound_inclusive)
    {
        return throw_if_uninitialized_at_perform()
            .obtain(pool_name)
            .obtain(schema_name)
            .obtain(collection_name)
            .obtain_between(lower_bound, upper_bound, lower_bound_inclusive, upper_bound_inclusive);
    }

    db_server *dispose(
        std::string const &pool_name,
        std::string const &schema_name,
        std::string const &collection_name,
        tkey const &key)
    {
        throw_if_uninitialized_at_perform()
            .obtain(pool_name)
            .obtain(schema_name)
            .obtain(collection_name)
            .dispose(key);

        return this;
    }

private:

    db_server &throw_if_uninutialized(
        mode mode,
        std::string const &exception_message)
    {
        if (mode != mode::uninitialized)
        {
            return *this;
        }

        throw std::logic_error(exception_message);
    }

    db_server &throw_if_uninitialized_at_setup(
        mode mode)
    {
        return throw_if_uninutialized(mode, "invalid mode");
    }

    db_server &throw_if_initialized_at_setup()
    {
        if (_mode == mode::uninitialized)
        {
            return *this;
        }

        throw std::logic_error("attempt to change previously set up mode");
    }

    db_server &throw_if_uninitialized_at_perform()
    {
        return throw_if_uninutialized(_mode, "attempt to perform an operation while mode not initialized");
    }

public:

    db_server *set_mode(
        mode mode)
    {
        throw_if_initialized_at_setup()
            .throw_if_uninitialized_at_setup(mode);

        _mode = mode;

        return this;
    }

private:

    db_server &throw_if_invalid_path(
        std::string const &subpath)
    {
        if (_mode == mode::file_system)
        {
            // TODO: you can do it O_o
        }

        return *this;
    }

    db_server &throw_if_invalid_file_name(
        std::string const &file_name)
    {
        if (_mode == mode::file_system)
        {
            // TODO: you can do it O_o
        }

        return *this;
    }

    db_server &throw_if_path_is_too_long(
        std::string const &pool_name,
        std::string const &schema_name,
        std::string const &collection_name)
    {
        if (_mode == mode::file_system)
        {
            // TODO: you can do it O_o
        }

        return *this;
    }

};