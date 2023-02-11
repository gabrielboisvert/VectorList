
#include "pch.h"
#include <iostream>

#include "catch.hpp"

#include "SpyAllocator.h"
#include "Foo.h"
#include "MonContainer.h"






#define DO(inst) std::printf("\n" #inst "\n"); inst

// Set to 0 to use your containers.
#define USE_STD 0

#if USE_STD
#include <vector>
#include <list>
#include <string>
using namespace std;
#else
#include "MyVector.h"
#include "MyList.h"
#include "MyString.h"
using namespace my;
#endif



TEST_CASE("MonContainer", "[VectorList]")
{
	{
		std::printf("===========Test container============\n");
		Foo::ResetCount();
		std::printf("Create container\n");

		MonContainer<Foo, SpyAllocator<Foo> > foos;

		CHECK(Foo::Count() == 5);

		std::printf("Destroy container\n");


		Foo::ResetCount();
	}

	g_memorySpy.CheckLeaks();
}




/* Test : push_back, capacity, size, operator[]. */
TEST_CASE("VectorInt", "[VectorList]")
{
	{
		std::printf("=======Vector of int================\n\nCreate vector\n");
		vector<int, SpyAllocator<int>>    intVec;

		std::printf("\nsizeof(intVec) : %llu\n", sizeof(intVec));
		std::printf("\nCapacity=%llu, Size=%llu\n", intVec.capacity(), intVec.size());

		DO(intVec.push_back((const int&)10);)
			DO(intVec.push_back((const int&)20);)
			DO(intVec.push_back((const int&)30);)
			DO(intVec.push_back((const int&)40);)
			DO(intVec.push_back((const int&)50);)

			std::printf("\nCapacity=%llu, Size=%llu\n", intVec.capacity(), intVec.size());

		DO(intVec[3] *= 2;)

			for (size_t i = 0; i < intVec.size(); ++i)
			{
				std::printf("%d ", intVec[i]);
			}
		std::printf("\n");

		std::printf("\nDestroy vector\n\n");
	}

	g_memorySpy.CheckLeaks();
}


/* Test : reserve. */
TEST_CASE("VectorFoo_Reserve", "[VectorList]")
{
	{
		Foo::ResetCount();

		std::printf("\n=======Vector of Foo================\n\nCreate vector\n");

		vector<Foo, SpyAllocator<Foo> > fooVec;

		std::printf("\nsizeof(fooVec) : %llu\n", sizeof(fooVec));

		DO(fooVec.reserve(2));
		DO(fooVec.push_back((const Foo&)Foo()));
		DO(fooVec.push_back((const Foo&)Foo()));
		DO(fooVec.push_back((const Foo&)Foo()));

		std::printf("\nDestroy vector\n\n");
	}

	g_memorySpy.CheckLeaks();

}



/* Test : reserve, clear */
TEST_CASE("VectorFoo_Clear", "[VectorList]")
{
	{
		Foo::ResetCount();

		std::printf("\n=======Vector of Foo================\n\nCreate vector\n");

		vector<Foo, SpyAllocator<Foo> > fooVec;

		DO(fooVec.reserve(2));

		DO(fooVec.push_back((const Foo&)Foo()));
		DO(fooVec.push_back((const Foo&)Foo()));
		DO(fooVec.clear());

		DO(fooVec.push_back((const Foo&)Foo()));

		std::printf("\nDestroy vector\n\n");
	}

	g_memorySpy.CheckLeaks();
}


/* Test : resize, copy ctor */
TEST_CASE("VectorFoo_resize", "[VectorList]")
{
	{
		Foo::ResetCount();

		std::printf("\n=======Vector of Foo================\n\nCreate vector\n");

		vector<Foo, SpyAllocator<Foo> > fooVec;

		DO(fooVec.resize(2));

		DO(fooVec.push_back((const Foo&)Foo()));
		DO(fooVec.push_back((const Foo&)Foo()));

		std::printf("\nDestroy vector\n\n");
	}

	g_memorySpy.CheckLeaks();
}


TEST_CASE("VectorFoo_copy_ctor", "[VectorList]")
{
	{
		Foo::ResetCount();

		std::printf("\n=======Vector of Foo================\n\nCreate vector\n");

		vector<Foo, SpyAllocator<Foo> > fooVec;

		DO(fooVec.push_back((const Foo&)Foo()));
		DO(fooVec.push_back((const Foo&)Foo()));

		std::printf("\nCopy fooVec\n\n");

		vector<Foo, SpyAllocator<Foo> > fooVec2 = fooVec;

		std::printf("\nDestroy vectors \n");
	}


	g_memorySpy.CheckLeaks();
}


TEST_CASE("VectorFoo_operator=", "[VectorList]")
{
	{
		Foo::ResetCount();

		std::printf("\n=======Vector of Foo================\n\nCreate vector\n");

		vector<Foo, SpyAllocator<Foo> > fooVec;

		DO(fooVec.push_back((const Foo&)Foo()));
		DO(fooVec.push_back((const Foo&)Foo()));

		std::printf("\nCopy fooVec\n\n");

		vector<Foo, SpyAllocator<Foo> > fooVec2;
		fooVec2 = fooVec;

		std::printf("\nDestroy vectors \n");
	}

	g_memorySpy.CheckLeaks();
}



/* Test : begin, end, vector iterator operators */
TEST_CASE("Vector_Iterators", "[VectorList]")
{
	std::printf("\n=======Vector_Iterators================\n");

	Foo::ResetCount();

	{
		vector<Foo> fooVec;

		vector<Foo>::iterator itBeg = fooVec.begin();
		vector<Foo>::iterator itEnd = fooVec.end();

		REQUIRE(itBeg == itEnd);
		REQUIRE(fooVec.size() == 0);

		DO(fooVec.push_back((const Foo&)Foo()));
		DO(fooVec.push_back((const Foo&)Foo()));
		DO(fooVec.push_back((const Foo&)Foo()));


		std::printf("\nPrint vector:\n");
		for (vector<Foo>::iterator it = fooVec.begin(); it != fooVec.end(); ++it)
		{
			std::printf("%d ", (*it).MyCount());
		}
		std::printf("\n\n");

		DO(vector<Foo>::iterator it = fooVec.begin());
		REQUIRE(it->MyCount() == 7);

		it = fooVec.end() - 1;
		REQUIRE(it->MyCount() == 6);

		it = fooVec.begin() + 1;
		REQUIRE(it->MyCount() == 8);

		it += 1;
		REQUIRE(it->MyCount() == 6);

		it--;
		REQUIRE(it->MyCount() == 8);

		vector<Foo>::iterator itCpy = it++;
		REQUIRE(itCpy->MyCount() == 8);
		REQUIRE(itCpy != it);

		it -= 1;
		REQUIRE(it->MyCount() == 8);

		vector<Foo>::iterator itFound = std::find_if(fooVec.begin(), fooVec.end(), [](const Foo& foo) { return (foo.MyCount() == 8); });
		REQUIRE(itFound != fooVec.end());
		REQUIRE(itFound->MyCount() == 8);

		itFound = std::find_if(fooVec.begin(), fooVec.end(), [](const Foo& foo) { return (foo.MyCount() == 0xD0D0CACA); });
		REQUIRE(itFound == fooVec.end());

		std::printf("\nCheck Iterators: OK\n");
	}

	g_memorySpy.CheckLeaks();


}



/* Test : emplace_back, move ctor, move assignment operator */
TEST_CASE("Vector_Rvalues_Variadics", "[VectorList]")
{
	std::printf("\n=======Vector_Rvalues_Variadics================\n");

	Foo::ResetCount();

	{
		vector<Foo> fooVec;

		// should call push_back(val &&).
		DO(fooVec.push_back(Foo()));
		DO(fooVec.push_back(Foo()));
		DO(fooVec.push_back(Foo()));

		int currCount = Foo::Count();
		size_t fooSize = fooVec.size();
		size_t fooCap = fooVec.capacity();
		Foo* firstFooPtr = &fooVec[0];

		// should call move constructor
		DO(vector<Foo> thiefVector = std::move(fooVec));

		REQUIRE(Foo::Count() == currCount);

		REQUIRE(fooVec.size() == 0);
		REQUIRE(fooVec.capacity() == 0);

		REQUIRE(thiefVector.size() == fooSize);
		REQUIRE(thiefVector.capacity() == fooCap);
		REQUIRE(&thiefVector[0] == firstFooPtr);


		vector<Foo> anotherFooVec;
		DO(anotherFooVec.push_back(Foo()));
		DO(anotherFooVec.push_back(Foo()));
		DO(anotherFooVec.push_back(Foo()));

		currCount = Foo::Count();
		fooSize = anotherFooVec.size();
		fooCap = anotherFooVec.capacity();
		firstFooPtr = &anotherFooVec[0];


		// should call move operator =.
		DO(thiefVector = std::move(anotherFooVec););

		REQUIRE(anotherFooVec.size() == 0);
		REQUIRE(anotherFooVec.capacity() == 0);

		REQUIRE(thiefVector.size() == fooSize);
		REQUIRE(thiefVector.capacity() == fooCap);
		REQUIRE(&thiefVector[0] == firstFooPtr);

		thiefVector.emplace_back(4242);
	}

	g_memorySpy.CheckLeaks();
}



/* List.insert, remove, push_back, begin, end, size */
TEST_CASE("ListInt", "[VectorList]")
{
	{
		std::printf("\n=======List================\n\nCreate list\n");

		list<int, SpyAllocator<int>> maList;

		DO(maList.push_back((const int&)10));
		DO(maList.push_back((const int&)20));
		DO(maList.push_back((const int&)30));
		DO(maList.push_back((const int&)40));
		DO(maList.push_back((const int&)50));

		std::printf("\nList size : %llu\n", maList.size());


		std::printf("\nPrint list:\n");
		for (list<int, SpyAllocator<int>>::iterator it = maList.begin(); it != maList.end(); ++it)
		{
			std::printf("%d ", *it);
		}
		std::printf("\n\n");

		
		DO(maList.remove(20));
		DO(maList.remove(40));

		for (list<int, SpyAllocator<int>>::iterator it = maList.begin(); it != maList.end(); ++it)
		{
			std::printf("%d ", *it);
		}
		std::printf("\n\n");

		
		DO(maList.insert(std::find(maList.begin(), maList.end(), 30), 60));

		std::printf("\nList size : %llu\n", maList.size());
		
		std::printf("\nPrint list:\n");
		for (list<int, SpyAllocator<int>>::iterator it = maList.begin(); it != maList.end(); ++it)
		{
			std::printf("%d ", *it);
		}
		std::printf("\n\n");

		std::printf("\nDestroy list\n\n");
	}

	g_memorySpy.CheckLeaks();

}


/* List.copy ctor */
TEST_CASE("List_CopyCtor", "[VectorList]")
{
	{
		std::printf("\n=======List================\n\nCreate list\n");

		list<int, SpyAllocator<int>> list1;

		DO(list1.push_back((const int&)10));
		DO(list1.push_back((const int&)20));
		DO(list1.push_back((const int&)30));
		DO(list1.push_back((const int&)40));
		DO(list1.push_back((const int&)50));


		std::printf("\n\nCopy list = list2\n");
		list<int, SpyAllocator<int>> list2 = list1;

		DO(list1.remove(20));
		DO(list1.remove(40));
		DO(list1.insert(std::find(list1.begin(), list1.end(), 30), 60));



		std::printf("\nPrint list:\n");
		for (list<int, SpyAllocator<int>>::iterator it = list1.begin(); it != list1.end(); ++it)
		{
			std::printf("%d ", *it);
		}
		std::printf("\n\n");

		std::printf("\nPrint list2:\n");
		for (list<int, SpyAllocator<int>>::iterator it = list2.begin(); it != list2.end(); ++it)
		{
			std::printf("%d ", *it);
		}
		std::printf("\n\n");

		std::printf("\nDestroy lists\n\n");
	}

	g_memorySpy.CheckLeaks();
}



TEST_CASE("List_Iterators", "[VectorList]")
{
	std::printf("\n=======List_Iterators================\n");

	Foo::ResetCount();

	{
		list<Foo> fooList;

		list<Foo>::iterator itBeg = fooList.begin();
		list<Foo>::iterator itEnd = fooList.end();

		REQUIRE(itBeg == itEnd);
		REQUIRE(fooList.size() == 0);

		DO(fooList.push_back((const Foo&)Foo()));
		DO(fooList.push_back((const Foo&)Foo()));
		DO(fooList.push_back((const Foo&)Foo()));

		std::printf("\nPrint list:\n");
		for (list<Foo>::iterator it = fooList.begin(); it != fooList.end(); ++it)
		{
			std::printf("%d ", (*it).MyCount());
		}
		std::printf("\n\n");

		DO(list<Foo>::iterator itList = fooList.begin(););
		REQUIRE(itList->MyCount() == 1);

		itList++;
		REQUIRE(itList->MyCount() == 3);

		++itList;
		REQUIRE(itList->MyCount() == 5);

		itList--;
		REQUIRE(itList->MyCount() == 3);

		list<Foo>::iterator itCpy = itList++;
		REQUIRE(itCpy->MyCount() == 3);
		REQUIRE(itList->MyCount() == 5);
		REQUIRE(itCpy != itList);


		list<Foo>::iterator itFound = std::find_if(fooList.begin(), fooList.end(), [](const Foo& foo) { return (foo.MyCount() == 0xBAAAAAAD); });
		REQUIRE(itFound == fooList.end());

		// -- on end() iterator : should assert/exception/crash
		// itFound++;


		itFound = std::find_if(fooList.begin(), fooList.end(), [](const Foo& foo) { return (foo.MyCount() == 3); });
		REQUIRE(itFound != fooList.end());
		REQUIRE(itFound->MyCount() == 3);


		// Checking all ways to insert

		itFound = fooList.insert(itFound, (const Foo&)Foo());
		REQUIRE(itFound->MyCount() == 7);
		REQUIRE((++itFound)->MyCount() == 3);

		itFound = fooList.insert(itFound, 3, (const Foo&)Foo());
		REQUIRE(itFound->MyCount() == 9);
		REQUIRE((++itFound)->MyCount() == 10);
		REQUIRE((++itFound)->MyCount() == 11);
		REQUIRE((++itFound)->MyCount() == 3);
		REQUIRE((++itFound)->MyCount() == 5);
		REQUIRE((++itFound) == fooList.end());

		itFound = fooList.begin();
		vector<Foo> testInsertVector;
		testInsertVector.reserve(3);
		testInsertVector.push_back((const Foo&)Foo());
		testInsertVector.push_back((const Foo&)Foo());
		testInsertVector.push_back((const Foo&)Foo());

		itFound = fooList.insert(fooList.begin(), testInsertVector.begin(), testInsertVector.end());
		REQUIRE(itFound == fooList.begin());
		REQUIRE((itFound)->MyCount() == 18);
		REQUIRE((++itFound)->MyCount() == 19);
		REQUIRE((++itFound)->MyCount() == 20);
		REQUIRE((++itFound)->MyCount() == 1);

		std::printf("\nCheck Iterators: OK\n");
	}

	g_memorySpy.CheckLeaks();
}


TEST_CASE("List_Rvalues_Variadics", "[VectorList]")
{
	std::printf("\n=======List_Rvalues_Variadics================\n");

	Foo::ResetCount();

	{
		list<Foo> fooList;

		// should call push_back(T &&).
		DO(fooList.push_back(Foo()));
		DO(fooList.push_back(Foo()));
		DO(fooList.push_back(Foo()));

		int currCount = Foo::Count();
		size_t fooSize = fooList.size();
		Foo* firstFooPtr = &(*fooList.begin());

		// should call move ctor.
		list<Foo> thiefList = std::move(fooList);

		REQUIRE(Foo::Count() == currCount);

		REQUIRE(fooList.size() == 0);
		REQUIRE(fooList.begin() == fooList.end());

		REQUIRE(thiefList.size() == fooSize);
		REQUIRE(&(*thiefList.begin()) == firstFooPtr);


		DO(fooList.push_back(Foo()));
		DO(fooList.push_back(Foo()));
		DO(fooList.push_back(Foo()));

		currCount = Foo::Count();
		fooSize = fooList.size();
		firstFooPtr = &(*fooList.begin());

		// should call move operator=.
		thiefList = std::move(fooList);

		REQUIRE(Foo::Count() == currCount);

		REQUIRE(fooList.size() == 0);
		REQUIRE(fooList.begin() == fooList.end());

		REQUIRE(thiefList.size() == fooSize);
		REQUIRE(&(*thiefList.begin()) == firstFooPtr);

		fooList.emplace_back(6000);
	}

	g_memorySpy.CheckLeaks();
}


/* String */
TEST_CASE("String", "[VectorList]")
{
	std::printf("\n=======Strings================\n");

	{

		std::printf("Sizeof String : %llu\n", sizeof(string));

		{
			DO(string s);
			std::printf("Length of s : %llu\n", s.length());
		}

		{
			DO(string s = "Moins de 16 car");
			std::printf("Length of s : %llu\n", s.length());
		}

		{
			DO(string s = "Plus de 16 car, 27 au total");
			std::printf("Length of s : %llu\n", s.length());
		}

		{
			DO(string s = "Plus de 32 car, 34 pour etre exact");
			std::printf("Length of s : %llu\n", s.length());
		}

		{
			DO(string s = "Plus de 48 car, 51 pour etre exact, bon ca suffit ?");
			std::printf("Length of s : %llu\n", s.length());
		}

		{
			DO(string s1 = "toto est vraiment une personne incroyable");
			DO(string s2 = s1);
			DO(s1[1] = 'a');
			DO(s1[3] = 'a');
			std::printf("s1 : %s\n", s1.c_str());
			std::printf("s2 : %s\n", s2.c_str());
			DO(s2 = s1);
			DO(s2[3] = 'i');
			std::printf("s1 : %s\n", s1.c_str());
			std::printf("s2 : %s\n", s2.c_str());
		}

		DO(string s1 = "Bonjour");
		DO(string s2 = " le monde !");
		DO(string s3 = s1 + s2);
		std::printf("s3 : %s\n", s3.c_str());


		std::printf("\nDestroy vector\n\n");
	}

	g_memorySpy.CheckLeaks();
}

/* additional Vector test */
TEST_CASE("custom Vector", "[VectorList]")
{
	std::printf("\n=======More Vector Test================\n");

	{
		{
			vector<int, SpyAllocator<int>> vec;
			DO(vec.push_back(1));
			DO(vec.push_back(2));
			DO(vec.push_back(3));
			DO(vec.push_back(4));
			DO(vec.push_back(5));

			//Should trigger exception in debug only
			//DO(vec[-1]);
			
			std::printf("\nReserve lower than capacity should do nothin:\n");
			//Should trigger exception in debug only
			//DO(vec.reserve(-2));
			DO(vec.reserve(2));

			std::printf("\nReserve higher than capacity should realloc:\n");
			DO(vec.reserve(10));


			std::printf("\nresize higher than capacity should realloc and create empty T:\n");
			//Should trigger exception in debug only
			//DO(vec.resize(-2));
			DO(vec.resize(12));

			DO(REQUIRE((vec.capacity() == 12)));
			DO(REQUIRE((vec.size() == 12)));

			std::printf("\nPrint list:\n");
			for (vector<int, SpyAllocator<int>>::iterator it = vec.begin(); it != vec.end(); ++it)
			{
				std::printf("%d ", *it);
			}
			std::printf("\n\n");

			std::printf("\nresize lower than size element should delete T:\n");
			DO(vec.resize(3));
			
			DO(REQUIRE((vec.capacity() == 12)));
			DO(REQUIRE((vec.size() == 3)));

			std::printf("\nPrint list:\n");
			for (vector<int, SpyAllocator<int>>::iterator it = vec.begin(); it != vec.end(); ++it)
			{
				std::printf("%d ", *it);
			}
			std::printf("\n\n");
		}

		{
			//Check in for no reallocation of equal capacity
			vector<int, SpyAllocator<int>> vec;
			vector<int, SpyAllocator<int>> vec2;

			DO(vec.reserve(3));
			DO(vec.push_back(1));
			DO(vec.push_back(2));
			DO(vec.push_back(3));
			
			DO(vec2.reserve(3));

			std::printf("\nShould not deallocate or realocate:\n");
			DO(vec2 = vec);
			std::printf("\nPrint list:\n");
			for (vector<int, SpyAllocator<int>>::iterator it = vec2.begin(); it != vec2.end(); ++it)
			{
				std::printf("%d ", *it);
			}
			std::printf("\n\n");
		}
	}

	g_memorySpy.CheckLeaks();
}

/* additional List test */
TEST_CASE("custom List", "[VectorList]")
{
	std::printf("\n=======More List Test================\n");

	{
		{
			list<int, SpyAllocator<int>>::iterator it;
			list<int, SpyAllocator<int>> list1;
			DO(list1.push_back(1));
			DO(list1.push_back(2));
			DO(list1.push_back(3));
			DO(list1.push_back(4));
			DO(list1.push_back(5));
			std::printf("\nPrint list:\n");
			for (list<int, SpyAllocator<int>>::iterator it = list1.begin(); it != list1.end(); ++it)
			{
				std::printf("%d ", *it);
			}
			std::printf("\n\n");

			//Remove head
			std::printf("\nRemove Head 1:\n");
			DO(list1.remove(1));

			std::printf("\nPrint list:\n");
			for (list<int, SpyAllocator<int>>::iterator it = list1.begin(); it != list1.end(); ++it)
			{
				std::printf("%d ", *it);
			}
			std::printf("\n\n");
			//Remove tail
			std::printf("\nRemove tail 5:\n");
			DO(list1.remove(5));

			std::printf("\nPrint list:\n");
			for (list<int, SpyAllocator<int>>::iterator it = list1.begin(); it != list1.end(); ++it)
			{
				std::printf("%d ", *it);
			}
			std::printf("\n\n");

			//Remove multiple
			std::printf("\nRemove all element of 5:\n");
			DO(list1.push_back(5));
			DO(list1.push_back(5));
			DO(list1.push_back(5));
			DO(list1.push_back(5));
			std::printf("\nPrint list before remove:\n");
			for (list<int, SpyAllocator<int>>::iterator it = list1.begin(); it != list1.end(); ++it)
			{
				std::printf("%d ", *it);
			}
			std::printf("\n\n");

			DO(list1.remove(5));

			std::printf("\nPrint list after remove:\n");
			for (list<int, SpyAllocator<int>>::iterator it = list1.begin(); it != list1.end(); ++it)
			{
				std::printf("%d ", *it);
			}
			std::printf("\n\n");

			//Insert head
			std::printf("Insert head 1:\n");
			DO(it = list1.insert(list1.begin(), 1));
			REQUIRE(*it == 1);
			std::printf("\nPrint list:\n");
			for (list<int, SpyAllocator<int>>::iterator it = list1.begin(); it != list1.end(); ++it)
			{
				std::printf("%d ", *it);
			}
			std::printf("\n\n");
			//Insert tail
			std::printf("Insert head 5:\n");
			DO(it = list1.insert(list1.end(), 5));
			REQUIRE(*it == 5);

			std::printf("\nPrint list:\n");
			for (list<int, SpyAllocator<int>>::iterator it = list1.begin(); it != list1.end(); ++it)
			{
				std::printf("%d ", *it);
			}
			std::printf("\n\n");

			//Insert multiple head
			std::printf("Insert multiple head -1:\n");
			DO(it = list1.insert(list1.begin(), size_t(3), -1));
			DO(REQUIRE(*it == -1));
			DO(REQUIRE(*(++it) == -1));
			DO(REQUIRE(*(++it) == -1));
			DO(REQUIRE(*(++it) == 1));
			std::printf("\nPrint list:\n");
			for (list<int, SpyAllocator<int>>::iterator it = list1.begin(); it != list1.end(); ++it)
			{
				std::printf("%d ", *it);
			}
			std::printf("\n\n");
			//Insert multiple tail
			std::printf("Insert multiple tail -5:\n");
			DO(it = list1.insert(list1.end(), size_t(3), -5));
			DO(REQUIRE(*it == -5));
			DO(REQUIRE(*(++it) == -5));
			DO(REQUIRE(*(++it) == -5));
			DO(REQUIRE((++it) == nullptr));
			std::printf("\nPrint list:\n");
			for (list<int, SpyAllocator<int>>::iterator it = list1.begin(); it != list1.end(); ++it)
			{
				std::printf("%d ", *it);
			}
			std::printf("\n\n");

			//Insert iterator range
			vector<int> vec;
			DO(vec.push_back(1));
			DO(vec.push_back(2));
			DO(vec.push_back(3));

			DO(it = list1.insert(list1.end(), vec.begin(), vec.end()));
			std::printf("\nPrint list:\n");
			for (list<int, SpyAllocator<int>>::iterator it = list1.begin(); it != list1.end(); ++it)
			{
				std::printf("%d ", *it);
			}
			std::printf("\n\n");

			//Insert iterator range reverse
			//Should throw exception
			/*DO(it = list1.insert(list1.begin(), vec.end(), vec.begin()));
			std::printf("\nPrint list:\n");
			for (list<int, SpyAllocator<int>>::iterator it = list1.begin(); it != list1.end(); ++it)
			{
				std::printf("%d ", *it);
			}
			std::printf("\n\n");*/

			//Insert iterator range from different container
			//Should throw exception
			/*vector<int> vec2;
			DO(vec2.push_back(-1));
			DO(vec2.push_back(-2));
			DO(vec2.push_back(-3));
			DO(it = list1.insert(list1.begin(), vec.begin(), vec2.begin()));
			std::printf("\nPrint list:\n");
			for (list<int, SpyAllocator<int>>::iterator it = list1.begin(); it != list1.end(); ++it)
			{
				std::printf("%d ", *it);
			}
			std::printf("\n\n");*/
		}

		{
			list<int, SpyAllocator<int>> list1;
			DO(list1.push_back(1));

			list1.remove(1);
			list1.remove(5);
		}
	}

	g_memorySpy.CheckLeaks();
}

/* additional string test */
TEST_CASE("custom String", "[VectorList]")
{
	std::printf("\n=======More String Test================\n");

	{
		{
			DO(string str = "allo");
			DO(REQUIRE(str[3] == 'o'));
			DO(REQUIRE(str[4] == '\0'));
			//Should throw exception in debug only
			//DO(REQUIRE(str[-1]));

			DO(string str2 = " Toto");
			std::printf("Should not deallocate or reallocate\n");
			DO(string str3 = str + str2);

			std::printf("str3 : %s\n", str3.c_str());
			DO(REQUIRE(str3.length() == 9));
		}

		{
			DO(string str = "Toto");
			DO(string str2 = " est alle a la plage");
			DO(string str3 = "Garbage data to destroy.");
			
			DO(str3 = str + str2);
			std::printf("str3 : %s\n", str3.c_str());
			DO(REQUIRE(str3.length() == 24));
		}

		{
			DO(string str = "ceci est 16 car");
			std::printf("str3 : %s\n", str.c_str());
			DO(REQUIRE(str.length() == 15));
			DO(REQUIRE(str[14] == 'r'));
			DO(REQUIRE(str[15] == '\0'));
		
			//throw exception in debug only
			//DO(REQUIRE(str[16]));
		}

		{
			DO(string str = "ceci est 16 car.");
			std::printf("str3 : %s\n", str.c_str());
			DO(REQUIRE(str.length() == 16));
			DO(REQUIRE(str[14] == 'r'));
			DO(REQUIRE(str[15] == '.'));
			DO(REQUIRE(str[16] == '\0'));

			//throw exception in debug only
			//DO(REQUIRE(str[17]));
		}
	}

	g_memorySpy.CheckLeaks();
}


TEST_CASE("STD_WarningTest", "[!mayfail]")
{
#if USE_STD
	std::printf("!!!!!!!!!!! WARNING !!!!!!!!!!!\nYou're using the standard containers. Set USE_STD to 0 to test your own !\n");
#endif

	CHECK(!USE_STD);
}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started:
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
