#ifndef ERROR_H_
#define ERROR_H_

#define VOID_ERROR_HANDLER(assertion, call_description)				\
	do {								\
		if (assertion) {					\
			fprintf(stderr, "(%s, %d): ",			\
					__FILE__, __LINE__);		\
			perror(call_description);			\
            return; \
		}							\
	} while(0)

#define NONVOID_ERROR_HANDLER(assertion, call_description, return_value) \
	do {								\
		if (assertion) {					\
			fprintf(stderr, "(%s, %d): ",			\
					__FILE__, __LINE__);		\
			perror(call_description);			\
            return return_value; \
		}							\
	} while(0)

#endif
