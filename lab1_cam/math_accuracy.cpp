#include "stdlib.h"
void graph1() {

}

void graph2() {

}

float power(const float value, const float pow) {
	float result;
	if (pow == 0) return 0;
	if (pow == 1) return value;
	if (pow > 1) {
		for (int i = 0; i <= pow; ++i) {
			result *= value;
		}
	}
	else if (pow < 0) {
		for (int i = 0; i <= pow; ++i) {
			result /= value;
		}
	}
}