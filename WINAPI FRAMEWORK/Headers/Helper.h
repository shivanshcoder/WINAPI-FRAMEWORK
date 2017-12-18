#include<Windows.h>
#include<intrin.h>

//Function to place breakpoint in case of error
inline void CheckError () {
	char buf[256];
	FormatMessageA ( FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, ::GetLastError (), MAKELANGID ( LANG_NEUTRAL, SUBLANG_DEFAULT ),
		buf, sizeof ( buf ), NULL );
	if( strcmp ( buf, "The operation completed successfully.\r\n" ) ) {
		__debugbreak ();
	}
	return;
}