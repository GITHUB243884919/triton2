#pragma  once 
  
namespace lk 
{
 
struct compare_str{
	bool operator()(const char* p1, const char*p2) const{
		return strcmp(p1,p2)==0;
	}
};

};

