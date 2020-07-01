#include "evaluation_list.implementation.cpp"

class evaluation_list_implement : public evaluation_list
{
public:
	virtual library::type_info const & type() override { return library::type<evaluation_list_implement>(); }
	virtual unsigned long input_count() override { return inputs.size(); }
	virtual unsigned long output_count() override { return outputs.size(); }
	virtual unsigned long operation_count() override { return operations.size(); }

	virtual void * get_input(unsigned long index) override { return inputs[index].offset; }
	virtual void * get_output(unsigned long index) override { return outputs[index].offset; }
	virtual void set_input(unsigned long index, void * address, void * base) override { inputs[index].offset = address - base; }
	virtual void set_output(unsigned long index, void * address, void * base) override { outputs[index].offset = address - base; }
	virtual operation * get_operation(unsigned long index) override { return operations[index].operation; }
	virtual void set_operation(unsigned long index, ::operation * operation)
	{
		operations[index].operation = operation;
		operations[index].input_indices.resize(operation.input_count());
		operations[index].output_indices.resize(operation.output_count());
	}

	virtual library::type_info const & input_type(unsigned long index) override { return *inputs[index].type; }
	virtual library::type_info const & output_type(unsigned long index) override { return *outputs[index].type; }


	virtual typed_value * get_value(unsigned long value) override
	{
		if (value < inputs.size()) {
			return &inputs[value].storage;
		}
		value -= inputs.size();
		if (value < outputs.size()) {
			return &outputs[value].storage;
		}
		value -= outputs.size();
		return &storage[value];
	}

	virtual unsigned long get_input_value(unsigned long input) override { return input; }
	virtual unsigned long get_output_value(unsigned long output) override { return output + inputs.size(); }
	virtual unsigned long value_count() override { return storage.size() + outputs.size() + inputs.size(); }

	virtual unsigned long get_operation_input(unsigned long operation, unsigned long input) override { return operations[operation].input_indices[input]; }
	virtual unsigned long get_operation_output(unsigned long operation, unsigned long output) override { return operations[operation].output_indices[output]; }

	virtual void set_operation_input(unsigned long operation, unsigned long input, unsigned long value) override
	{
		auto & op = operations[operation];
		op.input_indices[input] = value;
		op.operation->set_input(input, get_value(get_operation_input_value(operation, input))->value(), this);
	}
	virtual void set_operation_output(unsigned long operation, unsigned long output, unsigned long value) override
	{
		auto & op = operations[operation];
		op.output_indices[output] = value;
		op.operation->set_output(output, get_value(get_operation_output_value(operation, output))->value(), this);
	}

	virtual void more_inputs(unsigned long how_many, library::type_info const & type) override
	{
		inputs.resize(inputs.size() + how_many);
		for (unsigned long i = inputs.size() - how_many; i < inputs.size(); ++ i) {
			inputs[i].storage = simple_typed_storage(type);
		}
	}
	virtual void more_outputs(unsigned long how_many, library::type_info const & type) override
	{
		outputs.resize(outputs.size() + how_many);
		for (unsigned long i = outputs.size() - how_many; i < outputs.size(); ++ i) {
			outputs[i].storage = simple_typed_storage(type);
		}
	}
	virtual void more_intermediates(unsigned long how_many, library::type_info const & type) override
	{
		storage.resize(storage.size() + how_many);
		for (unsigned long i = storage.size() - how_many; i < storage.size(); ++ i) {
			storage[i] = simple_typed_storage(type);
		}
	}
	virtual void more_operations(unsigned long how_many) override
	{
		operations.resize(operations.size() + how_many);
	}

	virtual void subcall(unsigned long operation) {
		operations[operation].operation->call();
	}
	
private:
	class mapped_operation {
		operation* operation;
		library::stack_vector<unsigned long, 4> input_indices;
		library::stack_vector<unsigned long, 1> output_indices;
	};
	class mapped_external {
		unsigned long offset;
		library::type_info * type;
		simple_typed_storage storage;
	};
	library::stack_vector<mapped_operation, 32> operations;
	library::stack_vector<mapped_external, 4> inputs;
	library::stack_vector<mapped_external, 1> outputs;
	library::stack_vector<simple_typed_storage, 32> storage;
};

int main()
{
}
