// SampleStack.h
#ifndef __SampleStack_h__
#define __SampleStack_h__

  #define SAMPLESTACK_MAXIMUM 10

  /*
   * SampleStack
   */

  class SampleStack {
    double stack[SAMPLESTACK_MAXIMUM];
    int sampleCount = 0;
    
    public:
      SampleStack::SampleStack();
      double average();
      int count();
      void empty();
      int push(double pushedVal);
  };


#endif

