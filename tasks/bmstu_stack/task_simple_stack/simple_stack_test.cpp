#include <gtest/gtest.h>
#include <string>
#include "bmstu_stack.h"

struct CountCopyMoveDefault
{
   public:
	CountCopyMoveDefault(int param, int param2, int param3)
	{
		int a = param + param2 + param3;
		(void)(a);
		++default_constructor_count;
	}
	CountCopyMoveDefault() { ++default_constructor_count; }
	CountCopyMoveDefault(const CountCopyMoveDefault&)
	{
		++copy_constructor_count;
	}
	CountCopyMoveDefault(CountCopyMoveDefault&&) noexcept
	{
		++move_constructor_count;
	}
	static void reset_counters()
	{
		copy_constructor_count = 0;
		move_constructor_count = 0;
		default_constructor_count = 0;
		assignment_copy_count = 0;
		assignment_move_count = 0;
	}
	CountCopyMoveDefault& operator=(const CountCopyMoveDefault&)
	{
		++assignment_copy_count;
		return *this;
	}
	CountCopyMoveDefault& operator=(CountCopyMoveDefault&&) noexcept
	{
		++assignment_move_count;
		return *this;
	}

	static int copy_constructor_count;
	static int move_constructor_count;
	static int default_constructor_count;
	static int assignment_copy_count;
	static int assignment_move_count;
};

int CountCopyMoveDefault::copy_constructor_count = 0;
int CountCopyMoveDefault::move_constructor_count = 0;
int CountCopyMoveDefault::default_constructor_count = 0;
int CountCopyMoveDefault::assignment_copy_count = 0;
int CountCopyMoveDefault::assignment_move_count = 0;

TEST(StackTest, DefaultConstructor)
{
	bmstu::stack<int> s;
	ASSERT_TRUE(s.empty());
	ASSERT_EQ(s.size(), 0u);
}

TEST(StackTest, DefaultConstructor2)
{
	bmstu::stack<CountCopyMoveDefault> s;
	ASSERT_TRUE(s.empty());
	ASSERT_EQ(s.size(), 0u);
}

TEST(StackTest, PushAndTop)
{
	bmstu::stack<int> s;
	s.push(10);
	ASSERT_FALSE(s.empty());
	ASSERT_EQ(s.size(), 1u);
	ASSERT_EQ(s.top(), 10);

	s.push(20);
	ASSERT_EQ(s.size(), 2u);
	ASSERT_EQ(s.top(), 20);

	s.push(30);
	ASSERT_EQ(s.size(), 3u);
	ASSERT_EQ(s.top(), 30);
}

TEST(StackTest, Emplace)
{
	bmstu::stack<CountCopyMoveDefault> s;
	CountCopyMoveDefault::reset_counters();

	s.emplace(1, 2, 3);
	ASSERT_FALSE(s.empty());
	ASSERT_EQ(s.size(), 1u);

	ASSERT_EQ(CountCopyMoveDefault::default_constructor_count, 1);
	ASSERT_EQ(CountCopyMoveDefault::copy_constructor_count, 0);
	ASSERT_EQ(CountCopyMoveDefault::move_constructor_count, 0);
	ASSERT_EQ(CountCopyMoveDefault::assignment_move_count, 0);

	s.emplace(4, 5, 6);
	ASSERT_EQ(s.size(), 2u);

	ASSERT_EQ(CountCopyMoveDefault::default_constructor_count, 2);
	ASSERT_EQ(CountCopyMoveDefault::copy_constructor_count, 0);
	ASSERT_EQ(CountCopyMoveDefault::move_constructor_count, 1);
	ASSERT_EQ(CountCopyMoveDefault::assignment_copy_count, 0);
	ASSERT_EQ(CountCopyMoveDefault::assignment_move_count, 0);
}

TEST(StackTest, PushLvalue)
{
	bmstu::stack<CountCopyMoveDefault> s;
	CountCopyMoveDefault::reset_counters();

	CountCopyMoveDefault obj;
	s.push(obj);
	ASSERT_EQ(s.size(), 1u);

	ASSERT_EQ(CountCopyMoveDefault::default_constructor_count, 1);
	ASSERT_EQ(CountCopyMoveDefault::copy_constructor_count, 1);
	ASSERT_EQ(CountCopyMoveDefault::assignment_copy_count, 0);
}

TEST(StackTest, PushRvalue)
{
	bmstu::stack<CountCopyMoveDefault> s;
	CountCopyMoveDefault::reset_counters();

	s.push(CountCopyMoveDefault());
	ASSERT_EQ(s.size(), 1u);

	ASSERT_EQ(CountCopyMoveDefault::default_constructor_count, 1);
	ASSERT_EQ(CountCopyMoveDefault::move_constructor_count, 1);
	ASSERT_EQ(CountCopyMoveDefault::assignment_copy_count, 0);
	ASSERT_EQ(CountCopyMoveDefault::assignment_move_count, 0);
}

TEST(StackTest, Pop)
{
	bmstu::stack<int> s;
	s.push(10);
	s.push(20);
	s.push(30);
	ASSERT_EQ(s.size(), 3u);

	s.pop();
	ASSERT_EQ(s.size(), 2u);
	ASSERT_EQ(s.top(), 20);

	s.pop();
	ASSERT_EQ(s.size(), 1u);
	ASSERT_EQ(s.top(), 10);

	s.pop();
	ASSERT_EQ(s.size(), 0u);
	ASSERT_TRUE(s.empty());
}

TEST(StackTest, PopEmpty)
{
	bmstu::stack<int> s;
	ASSERT_THROW(s.pop(), std::underflow_error);
}

TEST(StackTest, TopEmpty)
{
	bmstu::stack<int> s;
	ASSERT_THROW(s.top(), std::underflow_error);
}

TEST(StackTest, TopConst)
{
	bmstu::stack<int> s;
	s.push(10);
	s.push(20);

	const bmstu::stack<int>& const_ref = s;
	ASSERT_EQ(const_ref.top(), 20);
	ASSERT_EQ(const_ref.size(), 2u);
}

TEST(StackTest, Clear)
{
	bmstu::stack<int> s;
	s.push(10);
	s.push(20);
	s.push(30);
	ASSERT_EQ(s.size(), 3u);

	s.clear();
	ASSERT_EQ(s.size(), 0u);
	ASSERT_TRUE(s.empty());
}

TEST(StackTest, ClearEmpty)
{
	bmstu::stack<int> s;
	s.clear();
	ASSERT_EQ(s.size(), 0u);
	ASSERT_TRUE(s.empty());
}

TEST(StackTest, ClearAndReuse)
{
	bmstu::stack<int> s;
	s.push(10);
	s.push(20);
	s.clear();

	s.push(30);
	s.push(40);
	ASSERT_EQ(s.size(), 2u);
	ASSERT_EQ(s.top(), 40);
}

TEST(StackTest, MultipleOperations)
{
	bmstu::stack<int> s;

	for (int i = 0; i < 100; ++i)
	{
		s.push(i);
	}
	ASSERT_EQ(s.size(), 100u);
	ASSERT_EQ(s.top(), 99);

	for (int i = 0; i < 50; ++i)
	{
		s.pop();
	}
	ASSERT_EQ(s.size(), 50u);
	ASSERT_EQ(s.top(), 49);
}

TEST(StackTest, EmplaceWithPush)
{
	bmstu::stack<CountCopyMoveDefault> s;
	CountCopyMoveDefault::reset_counters();

	s.emplace(1, 2, 3);
	CountCopyMoveDefault obj;
	s.push(obj);
	s.push(CountCopyMoveDefault());

	ASSERT_EQ(s.size(), 3u);
	ASSERT_FALSE(s.empty());
}

TEST(StackTest, StringStack)
{
	bmstu::stack<std::string> s;
	s.push("Hello");
	s.push("World");
	s.emplace("Test");

	ASSERT_EQ(s.size(), 3u);
	ASSERT_EQ(s.top(), "Test");

	s.pop();
	ASSERT_EQ(s.top(), "World");

	s.pop();
	ASSERT_EQ(s.top(), "Hello");
}

TEST(StackTest, OptimizationCheck)
{
	bmstu::stack<CountCopyMoveDefault> s;
	CountCopyMoveDefault::reset_counters();

	s.push(CountCopyMoveDefault());
	ASSERT_EQ(CountCopyMoveDefault::default_constructor_count, 1);
	ASSERT_EQ(CountCopyMoveDefault::move_constructor_count, 1);
	ASSERT_EQ(CountCopyMoveDefault::copy_constructor_count, 0);
	ASSERT_EQ(CountCopyMoveDefault::assignment_copy_count, 0);
	ASSERT_EQ(CountCopyMoveDefault::assignment_move_count, 0);

	s.emplace(1, 2, 3);
	ASSERT_EQ(CountCopyMoveDefault::default_constructor_count, 2);
	ASSERT_EQ(CountCopyMoveDefault::move_constructor_count, 2);
	ASSERT_EQ(CountCopyMoveDefault::copy_constructor_count, 0);

	CountCopyMoveDefault obj;
	s.push(obj);
	ASSERT_EQ(CountCopyMoveDefault::default_constructor_count, 3);
	ASSERT_EQ(CountCopyMoveDefault::copy_constructor_count, 1);
	ASSERT_EQ(CountCopyMoveDefault::move_constructor_count, 4);

	s.push(CountCopyMoveDefault());
	ASSERT_EQ(CountCopyMoveDefault::default_constructor_count, 4);
	ASSERT_EQ(CountCopyMoveDefault::move_constructor_count, 8);

	ASSERT_EQ(s.size(), 4u);
	ASSERT_EQ(CountCopyMoveDefault::assignment_copy_count, 0);
	ASSERT_EQ(CountCopyMoveDefault::assignment_move_count, 0);
}

TEST(StackTest, CheckBraceSequence)
{
	auto checkBraceSequence = [](const std::string& str)
	{
		bmstu::stack<char> s;
		for (char ch : str)
		{
			if (ch == '(')
			{
				s.push(ch);
			}
			else if (ch == ')')
			{
				if (s.empty())
				{
					return false;
				}
				s.pop();
			}
		}
		return s.empty();
	};
	ASSERT_TRUE(checkBraceSequence("()"));
	ASSERT_TRUE(checkBraceSequence("(())"));
	ASSERT_TRUE(checkBraceSequence("()()"));
	ASSERT_TRUE(checkBraceSequence("(()())"));
	ASSERT_FALSE(checkBraceSequence("(()"));
	ASSERT_FALSE(checkBraceSequence("())"));
	ASSERT_FALSE(checkBraceSequence(")("));
}

struct DummyType
{
	DummyType(const std::string& str, int id)
	{
		name = str;
		uid = id;
		++counter;
		printf("DummyType(|%d|, name=\"%s\", id=%d)\n", counter, name.c_str(),
			   uid);
	}
	DummyType(const DummyType& other) : name(other.name), uid(other.uid)
	{
		++counter;
		printf("DummyType COPY(|%d|, name=\"%s\", id=%d)\n", counter,
			   name.c_str(), uid);
	}

	DummyType(DummyType&& other) noexcept
		: name(std::move(other.name)), uid(other.uid)
	{
		++counter;
		printf("DummyType MOVE(|%d|, name=\"%s\", id=%d)\n", counter,
			   name.c_str(), uid);
	}
	~DummyType()
	{
		printf("~DummyType(|%d|, name=\"%s\", id=%d)\n", counter, name.c_str(),
			   uid);
		--counter;
	}

	static int counter;
	std::string name;
	int uid;
};

int DummyType::counter = 0;

TEST(StackTest, NonTrivialObject)
{
	bmstu::stack<DummyType> stt1;
	stt1.emplace("Sasha", 10);
	stt1.emplace("Alice", 8);

	bmstu::stack<DummyType> stt2;
	stt2.emplace("Lesha", 20);
	stt2 = stt1;
}
TEST(Ruleoffive, Destructor){
	ASSERT_EQ(DummyType::counter, 0);
	{
		bmstu::stack<DummyType> st1;
		st1.emplace("Sanya", 1);
		st1.emplace("Vanya", 2);
	}
	EXPECT_EQ(DummyType::counter, 0);
}
TEST(Ruleoffive, Copyconstructor){
	DummyType::counter = 0;
	bmstu::stack<DummyType> st2;
	st2.emplace("Sanya", 1);
	st2.emplace("Vanya", 2);
	bmstu::stack<DummyType> copyst2(st2);
	EXPECT_EQ(copyst2.size(),2);
	EXPECT_EQ(copyst2.top().name, "Vanya");
	copyst2.pop();
	EXPECT_EQ(copyst2.size(),1);
	EXPECT_EQ(st2.size(),2);
}
TEST(Ruleoffive, Moveconstructor){
	DummyType::counter = 0;
	bmstu::stack<DummyType> st3;
	st3.emplace("Sanya", 1);
	bmstu::stack<DummyType> st3_1(std::move(st3));
	EXPECT_EQ(st3.empty(), true);
	EXPECT_EQ(st3_1.top().name, "Sanya");


}
TEST(Ruleoffive, Copyassignment){
	DummyType::counter = 0;
	bmstu::stack<DummyType> st4;
	bmstu::stack<DummyType> st4_1;
	st4.emplace("Sanya",1);
	st4_1.emplace("Vanya",2);
	st4 = st4_1;
	EXPECT_EQ(st4_1.size(),1);
	EXPECT_EQ(st4_1.top().name, "Vanya");


}
TEST(Ruleoffive, Moveassignment){
	DummyType::counter = 0;
	bmstu::stack<DummyType> st5;
	bmstu::stack<DummyType> st5_1;
	st5.emplace("Sanya",1);
	st5_1.emplace("Vanya",2);
	st5_1 = std::move(st5);
	EXPECT_EQ(st5.empty(), true);
	EXPECT_EQ(st5_1.top().name, "Sanya");
	st5.emplace("Alice", 666);
	EXPECT_EQ(st5.top().name,"Alice");


}