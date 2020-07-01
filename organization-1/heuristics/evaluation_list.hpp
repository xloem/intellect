

// an interface to a list of operations each produce a result, and accumulate shared data, one or more of which is a result


// an evaluation_list is basically a state that has a bunch of map_steps
// that operate on it

namespace library { struct type_info; }

class type_error {};

class typed_pointer
{
public:
	virtual library::type_info const & type() const = 0;
	virtual void * value() = 0;
	virtual void const * value() const = 0;

	template <typename Type> Type & get();
};

class simple_typed_pointer : public typed_pointer
{
public:
	template <typename Type>
	simple_typed_pointer(Type* pointer);
	simple_typed_pointer(library::type_info const & type, void * pointer = 0);

	virtual library::type_info const & type() const override final { return type_; }
	virtual void * value() override final { return pointer_; }
	virtual void const * value() const override final { return pointer_; }

	simple_typed_pointer operator=(typed_pointer const & other);

	void * const pointer_;
	library::type_info const & type_;
};

class typed_value : public typed_pointer
{
public:
	//virtual typed_value & operator=(typed_value && other) = 0;
	virtual typed_value & operator=(typed_pointer const & other) = 0;
	virtual ~typed_value() {}
};

template <unsigned long bytes = sizeof(void*)>
class simple_typed_storage : public typed_value
{
public:
	simple_typed_storage();
	simple_typed_storage(library::type_info const & type);
	simple_typed_storage(typed_value const & data);
	template <typename Type>
	simple_typed_storage(Type const & data);

	void clear();

	virtual library::type_info const & type() const final override;
	virtual void * value() final override;
	virtual void const * value() const final override;

	//virtual typed_value & operator=(typed_value && other) final override;
	virtual typed_value & operator=(typed_pointer const & other) final override;
	template<typename Type> simple_typed_storage & operator=(Type const & other);
	virtual ~simple_typed_storage() final override;

	unsigned char data[bytes];
private:
	library::type_info const * _type;
};

class operation
{
public:
	virtual library::type_info const & type() = 0;

	virtual unsigned long input_count() = 0;
	virtual unsigned long output_count() = 0;

	virtual library::type_info const & input_type(unsigned long index) = 0;
	virtual library::type_info const & output_type(unsigned long index) = 0;

	// the above are normal for all operations
	
	// one idea is to make everything castable to a 'callable' withappropraite parameters
	// this would then pass off the virtual function call
	
	// another idea might be to place the parameters in, directly,
	// rather than using offsets to a pointer base
	
	// another idea might be to normalize offset to a pointer base,
	// not change them, but use them ...  wrapping them ...

	// the below may need changing

	virtual void * get_input(unsigned long which, void * pointer_base = 0) = 0;
	virtual void * get_output(unsigned long which, void * pointer_base = 0) = 0;

	virtual void set_input(unsigned long which, void * address, void * pointer_base = 0) = 0;
	virtual void set_output(unsigned long which, void * address, void * pointer_base = 0) = 0;

	virtual void call(void * pointer_base = 0) {}

protected:
	virtual void more_inputs(unsigned long how_many, library::type_info const & type) {}
	virtual void more_outputs(unsigned long how_many, library::type_info const & type) {}
};

class evaluation_list : public operation
{
public:
	virtual unsigned long operation_count() = 0;

	virtual operation * get_operation(unsigned long index) = 0;
	virtual void set_operation(unsigned long index, operation * value) = 0;

	virtual typed_value * get_value(unsigned long value) = 0;
	virtual unsigned long value_count() = 0;

	virtual unsigned long get_input_value(unsigned long input) = 0;
	virtual unsigned long get_output_value(unsigned long output) = 0;
	//virtual unsigned long get_operation_output_value(unsigned long operation, unsigned long output) = 0;

	virtual unsigned long get_operation_input(unsigned long operation, unsigned long input) = 0;
	virtual unsigned long get_operation_output(unsigned long operation, unsigned long output) = 0;

	virtual void set_operation_input(unsigned long operation, unsigned long input, unsigned long value) = 0;
	virtual void set_operation_output(unsigned long operation, unsigned long output, unsigned long value) = 0;

	virtual void call(void * pointer_base = 0) override {
		unsigned long input_count = this->input_count();
		unsigned long operation_count = this->operation_count();
		unsigned long output_count = this->output_count();
		for (unsigned long input = 0; input < input_count; ++ input) {
			*get_value(get_input_value(input)) = simple_typed_pointer(input_type(input), get_input(input, pointer_base));
		}
		for (unsigned long index = 0; index < operation_count; ++ index) {
			subcall(index);
		}
		for (unsigned long output = 0; output < output_count; ++ output) {
			simple_typed_pointer(output_type(output), get_output(output, pointer_base)) = *get_value(get_output_value(output));
		}
	}

protected:
	virtual void more_intermediates(unsigned long how_many, library::type_info const & type) = 0;
	virtual void more_operations(unsigned long how_many) = 0;

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

#include <functional>

template <typename Result, typename... Parameters>
class std_function_operation : public operation
{
public:
	std_function_operation(std::function<Result(Parameters...)> function);

	virtual library::type_info const & type() override;
	
	virtual unsigned long input_count() override;
	virtual unsigned long output_count() override;

	virtual void * get_input(unsigned long which, void * pointer_base) override;
	virtual void * get_output(unsigned long which, void * pointer_base) override;

	virtual void set_input(unsigned long which, void * address, void * pointer_base) override;
	virtual void set_output(unsigned long which, void * address, void * pointer_base) override;

	virtual library::type_info const & input_type(unsigned long index) override;
	virtual library::type_info const & output_type(unsigned long index) override;

	virtual void call(void * pointer_base) override;

	std::function<Result(Parameters...)> function;
	long parameter_offsets[sizeof...(Parameters)];
	long result_offset;
	//unsigned char parameters[0 + ... + sizeof(Parameters)];
	//unsigned char return_[sizeof(Return)];
private:
	struct detail;
};

/*
#include <library/stackvector.hpp>
class std_function_evaluation_list : public evaluation_list
{
public:
	//j
private:
	library::stackvector<element_type, 16> values;

	library::stackvector<element_type, 1> outputs;
};
*/
