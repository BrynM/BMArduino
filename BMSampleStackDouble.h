// BMSampleStackDouble.h
#ifndef __BMSampleStackDouble_h__
#define __BMSampleStackDouble_h__

  // number of samples to keep
  #define SAMPLESTACK_MAXIMUM 10
  // set to 0 to disable
  // should be less than half SAMPLESTACK_MAXIMUM at the most extreme
  #define SAMPLESTACK_RESET_ON_EMPTY 4

  /*
   * BMSampleStackDouble
   */

  class BMSampleStackDouble {
    double stack[SAMPLESTACK_MAXIMUM];
    int sampleCount = 0;
    
    public:
      BMSampleStackDouble::BMSampleStackDouble();
      double average();
      int count();
      void empty();
      int push(double pushedVal);
  };


#endif

