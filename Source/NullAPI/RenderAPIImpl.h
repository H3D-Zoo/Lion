#pragma once

class RefCount
{
public:
	RefCount();

	int operator ++() { count += 1; return count; }
	int operator --() { count -= 1; return count; }
	bool OnlyOne() { return count == 1; }

private:
	int count;
};