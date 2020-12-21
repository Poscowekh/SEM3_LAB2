#ifndef PERSON_HPP
#define PERSON_HPP
#include <utility>
#include <time.h>
#include <string>

using namespace std;

class Person
{
    private:
        size_t id;
        string first_name;
        string middle_name;
        string last_name;
        time_t birth_date;
    public:
        Person()
        {
            id = birth_date = 0;
        };
        Person(const Person* other)
        {
            id = other->id;
            birth_date = other->birth_date;
            first_name = other->first_name;
            middle_name = other->middle_name;
            last_name = other->last_name;
        };
        Person(size_t new_id, time_t bdate, string f_name, string m_name, string l_name)
        {
            id = new_id;
            birth_date = bdate;
            first_name = f_name;
            middle_name = m_name;
            last_name = l_name;
        };
        size_t ID()
        {
            return id;
        };
        time_t BirthDate()
        {
            return birth_date;
        };
        string FirstName()
        {
            return first_name;
        };
        string MiddleName()
        {
            return middle_name;
        };
        string LastName()
        {
            return last_name;
        };
        ~Person(){};
};

#endif // PERSON_HPP
