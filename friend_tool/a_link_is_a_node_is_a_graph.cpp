using namespace std;

class one
{
public:
	//one topic;
	one way;
	one what;
	double degree;
	double confidence;

	shared_ptr<void *> matter;

	struct less_matter {
		bool operator()(one & a, one & b)
		{
			return a.matter < b.matter;
		}
	};

	struct less_degree {
		bool operator()(one & a, one & b)
		{
			return  a.degree < b.degree;
		}
	};

	vector<reference<one>> near;
	multimap<one*, size_t> near_by_way;

	vector<reference<one>> internear; // can fill with interlinks

	reference<one> clone();

	void morph_towards(reference<one> & other, double amount)
	{
		for (auto & friendly : near) {
			if (other.near_by_way.contains
		}
		// if other has stuff that we don't, we want to instantiate it, maybe.
	}
};
