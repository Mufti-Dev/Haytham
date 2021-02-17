/* vtable simulation to show virtual function call */

/*Author: Mufti Md*/
#include <stdio.h>

class Base
 {
	 virtual void a()
	 {
		 printf("Hello from Base.\n");
	 }
 };

 class Derived : public Base
 {
	 virtual void b()
	 {
		 printf("Hello from Derived.\n");
	 }
 };

int main()
{
	Derived *ptr = new Derived();
	void (**vt)() = *(void (***)())ptr;

	((void (*)()) vt[0])();
	((void (*)()) vt[1])();

	delete ptr;
	return 0;
}