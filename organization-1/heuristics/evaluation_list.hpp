

// an interface to a list of operations each produce a result, and accumulate shared data, one or more of which is a result


// an evaluation_list is basically a state that has a bunch of map_steps
// that operate on it

namespace std { class type_info; }

struct type
{
	std::type_info const * type_info;
	unsigned long size;

	template <typename T> type(T const &)
	: type_info(&typeid(T)),
	  size(sizeof(T))
	{ }
};

class typed_pointer
{
public:
	virtual ::type type()  = 0;
	virtual void * value() = 0;

	template <typename Type> Type & get();
};

class simple_typed_pointer : public typed_pointer
{
public:
	template <typename T>
	simple_typed_pointer(T* pointer)
	: pointer_(pointer),
          type_(T()),
	{ }

	virtual ::type type() override final { return type_; }
	virtual void * value() override final { return pointer_; }
	virtual unsigned long size() override final { return size_; }

	void * const pointer_;
	::type const type_;
};

class typed_value : public typed_pointer
{
public:
	//virtual typed_value & operator=(typed_value && other) = 0;
	virtual typed_value & operator=(typed_pointer const & other) = 0;
	virtual ~typed_value() = 0;
};

template <unsigned long bytes = sizeof(void*)>
class simple_typed_storage : public typed_value
{
public:
	template <typename Type>
	simple_typed_storage(Type const & data);

	void clear();

	virtual ::type type() final override;
	virtual void * value() final override;

	//virtual typed_value & operator=(typed_value && other) final override;
	virtual typed_value & operator=(typed_value const & other) final override;
	virtual ~typed_storage() final override;

	unsigned char data[bytes];
private:
	void ::type _type;
};

class operation
{
public:
	virtual ::type type() = 0;

	virtual unsigned long input_count() = 0;
	virtual unsigned long output_count() = 0;

	virtual void * get_input(unsigned long which) = 0;
	virtual void * get_output(unsigned long which) = 0;

	virtual void set_input(unsigned long which, void * address, void * base = 0) = 0;
	virtual void set_output(unsigned long which, void * address, void * base = 0) = 0;

	virtual unsigned long input_size(unsigned long index) = 0;
	virtual unsigned long output_size(unsigned long index) = 0;

	virtual void const * input_type(unsigned long index) = 0;
	virtual void const * output_type(unsigned long index) = 0;

	virtual void call(void * pointer_base = 0) {}

protected:
	virtual void more_inputs(unsigned long how_many, ::type const & type) {}
	virtual void more_output(unsigned long how_many, ::type const & type) {}
};

class evaluation_list : public operation
{
public:
	virtual unsigned long operation_count() = 0;

	virtual operation * operation(unsigned long index) = 0;

	virtual typed_value * get_value(unsigned long value) = 0;

	virtual unsigned long get_input_value(unsigned long input) = 0;
	virtual unsigned long get_operation_output_value(unsigned long operation, unsigned long output) = 0;

	// concern: how are return values typed?
	// 	will be handled by simple_typed_storage, shoul dwork

	virtual unsigned long get_operation_input(unsigned long operation, unsigned long input) = 0;
	virtual unsigned long get_operation_output(unsigned long operation, unsigned long output) = 0;

	virtual void set_operation_input(unsigned long operation, unsigned long input, unsigned long value) = 0;
	virtual void set_operation_output(unsigned long operation, unsigned long output, unsigned long value) = 0;

	void more_operations(unsigned long how_many) = 0;

	virtual void call(void * base) override {
		unsigned long input_count = this->input_count();
		unsigned long output_count = this->output_count();
		for (unsigned long input = 0; input < input_count; ++ input) {
			*get_value(get_input_value(input)) = get_input(input) + base
		}
		for (unsigned long index = 0; index < operation_size(); ++ index) {
			subcall(index);
		}
	}

protected:
	virtual void subcall(unsigned long operation) = 0;
};

// how connect operation with operation_for_list
// 	both need where-inputs-come-from, where-outputs-go-to
// 	operation() holds in local object, returns pointers to for changing
// 		we'd really like to say those come from other places.
// 		we'd kinda like to store tokens, and provide the tokens
// 		when calling.
// 		we could provide the addresses when calling
// 		we could also provide offset addresses
// 	would kinda like to provide an offset address for parameters
// 	and provide base offet when calling
// 		given you have to set up parameters anyway it makes sense

/*
class function_operation : public operation
{

	template <typename Result, typename... Parameters>
	function_operation(Result(*function)(Parameters...));
	virtual void const * type() override;
};

class std_function_operation : public operation
{

	template <typename Result, typename... Parameters>
	function_operation(std::functionResult(*function)(Parameters...));
	virtual void const * type() override;
};
*/

template <typename Result, typename... Parameters>
class std_function_operation : public operation
{
public:
	std_function_operation(std::function<Result(Parameters...)> function);

	virtual ::type type() override;
	
	virtual unsigned long input_count() override;
	virtual unsigned long output_count() override;

	virtual void * get_input(unsigned long which) override;
	virtual void * get_output(unsigned long which) override;

	virtual void set_input(unsigned long which, void * address, void * base) override;
	virtual void set_output(unsigned long which, void * address, void * base) override;

	virtual unsigned long input_size(unsigned long index) override;
	virtual unsigned long output_size(unsigned long index) override;

	virtual ::type input_type(unsigned long index) override;
	virtual ::type output_type(unsigned long index) override;

	virtual void call(void * pointer_base) override;

	std::function<Return(Parameters...)> function;
	void* parameters[sizeof...(Parameters)];
	Result* result;
	//unsigned char parameters[0 + ... + sizeof(Parameters)];
	//unsigned char return_[sizeof(Return)];
private:
	struct detail;
};

class std_function_evaluation_list : public evaluation_list
{
public:
	j
private:
	library::stack_vector<element_type, 16> values;

	library::stack_vector<element_type, 1> outputs;
};
