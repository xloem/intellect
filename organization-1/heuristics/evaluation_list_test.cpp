#include "evaluation_list.implementation.cpp"

class evaluation_list_implement : public evaluation_list
{
public:
	virtual ::type type() override { return ::type(evaluation_list_implement()); }
	virtual unsigned long input_count() override { return inputs.size(); }
	virtual unsigned long output_count() override { return outputs.size(); }
	virtual unsigned long operation_count() override { return operations.size(); }

	virtual void * get_input(unsigned long index) override { return inputs[index]; }
	virtual void * get_output(unsigned long index) override { return outputs[index]; }
	virtual void set_input(unsigned long index, void * address, void * base) override { inputs[index] = address - base; }
	virtual void set_output(unsigned long index, void * address, void * base) override { outputs[index] = address - base; }
	virtual operation * operation(unsigned long index) override { return &operations[index].operation; }

	virtual ::type input_type(unsigned long index) override { return inputs[index].type(); }
	virtual ::type output_type(unsigned long index) override { return outputs[index].type(); }

	virtual unsigned long get_operation_input(unsigned long operation, unsigned long input) override { return operations[operation].input_indices[input]; }
	virtual unsigned long get_operation_output(unsigned long operation, unsigned long output) override { return operations[operation].output_indices[output]; }

	virtual void set_operation_input(unsigned long operation, unsigned long input, unsigned long value) override
	{
		auto & op = operations[operation];
		op.input_indices[input] = value;
		op.operation->set_input(input, &op.input_indices[input], this);
	}
	virtual void set_operation_output(unsigned long operation, unsigned long output, unsigned long value) override
	{
		auto & op = operations[operation];
		op.output_indices[output] = value;
		op.operation->set_output(output, &op.output_indices[output], this);
	}

	virtual void more_inputs(unsigned long how_many) { inputs.resize(inputs.size() + how_many); input_storage.resize(inputs.size()); }
	virtual void more_outputs(unsigned long how_many) { outputs.resize(outputs.size() + how_many); }
	virtual void more_operations(unsigned long how_many) { operations.resize(operations.size() + how_many); }

	virtual void subcall(unsigned long operation) { }
	
private:
	class mapped_operation {
		operation* operation;
		library::stack_vector<unsigned long, 4> input_indices;
		library::stack_vector<unsigned long, 1> output_indices;
		simple_typed_storage output;
	};
	library::stack_vector<mapped_operation, 32> operations;
	library::stack_vector<simple_typed_storage, 4> input_storage;
	library::stack_vector<unsigned long, 4> inputs;
	library::stack_vector<unsigned long, 1> outputs;

	// it would be nice if our operation class held inputs and outputs
};

int main()
{
}
