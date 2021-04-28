
#include <string>
#include <iostream>
#include "NumberWithUnits.hpp"
#include <cmath>
#include <fstream>
#include <sstream>

const double EPS = 0.001;
using namespace std;

namespace ariel {
        static map<string, map<string,double> > my_map;
        NumberWithUnits::NumberWithUnits(double m_value, const string m_type){
            if(my_map.count(m_type)==0){
                throw invalid_argument {"unit does not exist in the text file."};
            }
            this->m_type = m_type;
            this->m_value = m_value;         
         }
    
        double exchange(string from,string to,double value){
            if (from==to) {
                return value;
             }
                try{
                    return my_map.at(from).at(to)*value;//if key dont match throw out of range exc
                }
                catch(const exception& e)
                {
                    throw invalid_argument {"Units types do not match"};
                }
            }
        
        void NumberWithUnits::read_units(ifstream& text_file){
            double value1,value2;
            string unit1,unit2,equal;
            while (text_file>>value1>>unit1>>equal>>value2>>unit2){
                my_map[unit1][unit2]=value2;
                my_map[unit2][unit1]=1/value2;


                for(auto &pair : my_map[unit2]) {
                    double value = my_map[unit1][unit2] * pair.second;
                     my_map[unit1][pair.first] = value;
                    my_map[pair.first][unit1] = 1/value;
                 }
                for(auto &pair : my_map[unit1]) {
                    double value = my_map[unit2][unit1] * pair.second;
                    my_map[unit2][pair.first] = value;
                    my_map[pair.first][unit2] = 1/value;
                }
            }
            
        }

        /* Accounting operators */
        NumberWithUnits operator + (const NumberWithUnits& units1,const NumberWithUnits& units2){// x=(a+b) ->FRIND FUNCTION
            double value=exchange(units2.m_type,units1.m_type,units2.m_value);
            value=value+units1.m_value;
            return NumberWithUnits{value,units1.m_type};
        } 
        NumberWithUnits operator - (const NumberWithUnits& units1,const NumberWithUnits& units2){// x=(a-b) ->FRIND FUNCTION
            double value=exchange(units2.m_type,units1.m_type,units2.m_value);
            value=units1.m_value-value;
            return NumberWithUnits{value,units1.m_type};
        } 
        NumberWithUnits NumberWithUnits ::operator + () {
            return NumberWithUnits{+this->m_value,this->m_type};
        }   
        NumberWithUnits NumberWithUnits ::operator - () {
            return NumberWithUnits{-this->m_value,this->m_type};
        }   
        NumberWithUnits& NumberWithUnits ::operator += (const NumberWithUnits& units1){// a=a+b
            this->m_value+=exchange(units1.m_type,this->m_type,units1.m_value);
            return *this;
        } 
        NumberWithUnits& NumberWithUnits ::operator -= (const NumberWithUnits& units1){// a=a-b
            this->m_value-=exchange(units1.m_type,this->m_type,units1.m_value);
            return *this;
        } 

        /* Comparison operators */
        bool NumberWithUnits :: operator > (const NumberWithUnits& units1) const{ 
            double value=exchange(units1.m_type,this->m_type,units1.m_value);
            return (this->m_value > value);
        }
        bool NumberWithUnits :: operator >= (const NumberWithUnits& units1) const{
            double value=exchange(units1.m_type,this->m_type,units1.m_value);
            return (this->m_value >= value);
        }
        bool NumberWithUnits :: operator < (const NumberWithUnits& units1) const{
            double value=exchange(units1.m_type,this->m_type,units1.m_value);
            return (this->m_value < value);
        }
        bool NumberWithUnits :: operator <= (const NumberWithUnits& units1) const{
            double value=exchange(units1.m_type,this->m_type,units1.m_value);
            return (this->m_value <= value);
        }

        bool NumberWithUnits :: operator == (const NumberWithUnits& units1) const{
            double value=exchange(units1.m_type,this->m_type,units1.m_value);
            if(abs(this->m_value-value)<EPS){
                return true;
            }
            return false;
        }

        bool NumberWithUnits :: operator != (const NumberWithUnits& units1) const{
            return !(*this == units1);
        }
        // prefix  - ++a
        NumberWithUnits& NumberWithUnits :: operator ++ (){
            ++(this->m_value);
            return *this;
        }      
        // postfix - a++
		const NumberWithUnits NumberWithUnits :: operator ++ (int){
            double value=this->m_value;
            this->m_value+=1;
            return NumberWithUnits{value,this->m_type};
        }   
        // prefix  - --a
        NumberWithUnits& NumberWithUnits :: operator -- (){
            --(this->m_value);
            return *this;
        }      
        // postfix - a--
		const NumberWithUnits NumberWithUnits :: operator -- (int){
            double value=this->m_value;
            this->m_value-=1;
            return NumberWithUnits{value,this->m_type};
        }   

        /* Multiplication operator */
        NumberWithUnits operator * (const NumberWithUnits& units1,double multi) {
            double value= units1.m_value*multi;
            return NumberWithUnits(value,units1.m_type);
        }
        NumberWithUnits operator * (double multi,const NumberWithUnits& units1) {
            double value= units1.m_value*multi;
            return NumberWithUnits(value,units1.m_type);
        }
        /* Input & Output operators */ //->FRIND FUNCTIONS
        ostream& operator <<(ostream& os,const NumberWithUnits& units){
            return os<<units.m_value<<"["<<units.m_type<<"]";
        } 
        istream& operator >>(istream& is,NumberWithUnits& units){
            char left,right;
            double value;
            string type;
            is>>value>>left>>type;
            if(type.find(']')!=string::npos)//check if "]" is in the string
            {
                size_t pos=type.find(']');
                type=type.substr(0,pos);
            }
            else {
                is>>right;
            }
            if(my_map.count(type)==0){
                throw invalid_argument {"unit does not exist in the text file."};
            }
            units.m_value=value;
            units.m_type=type;
            return is;
       
         }  

            NumberWithUnits::~NumberWithUnits(){};
    }



        
    



