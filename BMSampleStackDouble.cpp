#include "BMSampleStackDouble.h"

/*
 * BMSampleStackDouble
 */

BMSampleStackDouble::BMSampleStackDouble() {
  empty();
}

double BMSampleStackDouble::average() {
  if(sampleCount == 0) {
    return 0;
  }

  if(sampleCount == 1) {
    return stack[0];
  }

  double total = 0;

  for(int i = 0; i < sampleCount; i++) {
    if(stack[i] > 0) {
      total = total + stack[i];
    }
  }

  return total / sampleCount;
}

int BMSampleStackDouble::count() {
  return sampleCount;
}

void BMSampleStackDouble::empty() {
  sampleCount = 0;

  // fill with -1
  for(int i = 0; i < SAMPLESTACK_MAXIMUM; i++) {
    stack[i] = -1;
  }
}

int BMSampleStackDouble::push(double pushedVal) {
  int lastIndex = sampleCount - 1;
  
  if(SAMPLESTACK_RESET_ON_EMPTY > 0) {
    // @TODO make this count and use the above setting
    if((pushedVal < 1) && (stack[lastIndex] < 1) && (stack[lastIndex - 1] < 1) && (stack[lastIndex - 2] < 1)) {
      // four 0s in a row mean empty.
      empty();
      return 0;
    }
  }

  if(sampleCount < (SAMPLESTACK_MAXIMUM - 1)) {
    stack[sampleCount] = pushedVal;
    sampleCount++;

    return sampleCount - 1;
  }

  for(int i = 0; i < sampleCount; i++) {
    if(i == (sampleCount - 1)) {
      stack[i] = pushedVal;
      continue;
    }

    stack[i] = stack[i+1];
  }

  return lastIndex;
}

