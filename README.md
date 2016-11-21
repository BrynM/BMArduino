- [BMArduino](#bmarduino)
  * [Description](#description)
  * [Installation](#installation)
  * [Controlling Debug Mode](#controlling-debug-mode)
    + [Debug Level 2](#debug-level-2)
  * [Using Debug Mode](#using-debug-mode)
    + [`BM_DEBUGGING` or `BM_NOISY_DEBUGGING`](#bm_debugging-or-bm_noisy_debugging)
    + [`FUNC_MSG()`](#func_msg)
    + [`CLASS_MSG()`](#class_msg)
    + [A Note on Compiled Size](#a-note-on-compiled-size)
  * [Utility Methods](#utility-methods)
    + [`announce()`](#announce)
  * [`info()`](#info)
    + [`pin_activate()`](#pin_activate)
    + [`value_is_between()`](#value_is_between_)
    + [`value_is_near()`](#value_is_near)
    + [`wait_for_serial()`](#wait_for_serial)

# BMArduino

My small utility Arduino library.

## Description

This will create an object called `BM` containing some utility methods and basic debug output functionality. It also features some board identification functionality (by chip) available in the constant `__BM_BOARD__`.

## Installation

Either clone this repository to your libraries folder or download the Zip of the repository and add it using your IDE. If you clone the repository, you will find the library in the Arduino IDE under `Sketch`->`Include Library`->`BMArduino`.

If you're using the Arduino IDE, note that because the zip will be called "BMArduino-master.zip", your entry in the `Sketch`->`Include Library` menu will also be "BMArduino-master". This is a byproduct of GitHub and cannot be avoided without extra work (sorry - this is a hobby).

When the library is included, Arduino IDE inserts an include line for every header file. You only need the one for `#include <BMArduino.h>`. Any others that were added when you used the menu to include it can be removed.

## Controlling Debug Mode
Lots of these functions use `Serial` to print debug messages and there are two handy macros defined so you can add similar debug messages to your code.

By default debugging is turned on. If you should turn it off, the compiled size of your sketch will be smaller because the strings used for debug output won't be defined. To change it, set the preprocessor directive `BM_DEBUG_LEVEL` to an integer value in [`BMArduino.h`](/BrynM/BMArduino/blob/master/BMArduino.h). Any value greater than `0` will enable debug mode and `0` will disable it.

Debugging is turned on in the following code.
```cpp
#include <BMArduino.h>

void setup() {
  // use the BM serial implementation
  BM.wait_for_serial(true);
}

void loop() {
}
```

On the serial console you should see the following.

```
@0ms # BM::wait_for_serial() # Hardware Serial begun at 9600 baud.
```

Now let's turn debug mode off with `#define BM_DEBUG_LEVEL 0` in [`BMArduino.h`](/BrynM/BMArduino/blob/master/BMArduino.h).
```cpp
#include <BMArduino.h>

void setup() {
  // use the BM serial implementation
  BM.wait_for_serial(true);
}

void loop() {
}
```

With debugging turned off, there is no serial console output from calling [`BM.wait_for_serial()`](#wait_for_serial).

### Debug Level 2

There is a secondary level of debug mode if `BM_DEBUG_LEVEL` is set to `2` or greater. If that is so, an extra preprocessor directive is defined called `BM_NOISY_DEBUGGING`. This can be used interchangeably with `BM_DEBUGGING` as described in [Using Debug Mode](#using-debug-mode)->[`BM_DEBUGGING` or `BM_NOISY_DEBUGGING`](#bm_debugging-or-bm_noisy_debugging).

## Using Debug Mode

### `BM_DEBUGGING` or `BM_NOISY_DEBUGGING`

When debug mode enabled, the preprocessor directive `BM_DEBUGGING` gets defined. This constant can then be used to optionally include your own debug messages in the compiled result. 

```cpp
#include <BMArduino.h>

void setup() {
  // use the BM serial implementation
  BM.wait_for_serial(true);

  #ifdef BM_DEBUGGING
    Serial.print("This long string will only be compiled in if BM_DEBUGGING is defined. Turing debug mode off will save bytes!");
  #endif
}

void loop() {
}

```

Compiling the above with debugging enabled produces the following sketch size.

```
Sketch uses 2,298 bytes (7%) of program storage space. Maximum is 32,256 bytes.
Global variables use 485 bytes (23%) of dynamic memory, leaving 1,563 bytes for local variables. Maximum is 2,048 bytes.
```

With debugging turned off, that entire large string is excluded from the final sketch size.

```
Sketch uses 1,688 bytes (5%) of program storage space. Maximum is 32,256 bytes.
Global variables use 273 bytes (13%) of dynamic memory, leaving 1,775 bytes for local variables. Maximum is 2,048 bytes.
```
 
### `FUNC_MSG()`

I assume that you don't plop all of your code into `setup()` and `loop()` (and if you do you are a barbarian). If you noticed the fancy formatting for the message when using "the BM serial implementation" (`BM.wait_for_serial()`), that style of output can be had for your debug messages as well. Two variations of `BM.announce()` are encapsulated in preprocessor macros. The first is `FUNC_MSG()`. This will open a line of serial output stating the `millis()` timestamp, a separator, the name of your function, and then a closing separator. 

Note that `FUNC_MSG()` and `BM.announce()` only open a line of output. It's up to you to send the ending newline to serial.

```cpp
#include <BMArduino.h>

void setup() {
  // use the BM serial implementation
  BM.wait_for_serial(true);

  my_clever_function();
}

void loop() {
}

void my_clever_function() {
  char myCleverString[8] = "Hi!";

  #ifdef BM_DEBUGGING
    FUNC_MSG();
    // Let's print the output of a variable.
    Serial.print("The value of myCleverString is: ");
    Serial.print(myCleverString);
    // note sending the newline
    Serial.println(" Ran the function!");
  #endif
}
```

The above code will produce something like the following output.

```
@0ms # BM::wait_for_serial() # Hardware Serial begun at 9600 baud.
@6ms # my_clever_function() # Hi! Ran the function!
```

Underneath, `FUNC_MSG()` is utilising the built in `__func__` preprocessor directive to hand to `BM.announce()`. It is the exact equivalent of calling `BM.announce(__func__);`

### `CLASS_MSG()`

So you're not a barbarian after all and even go as far as class abstraction. Good for you! The other variation of `BM.announce()` wrapped in a macro is `CLASS_MSG()`.

Like `FUNC_MSG()`, `CLASS_MSG()` will open a line of output to serial. However, there is no preprocessor macro (that I know of) for class names, so you must hand `CLASS_MSG()` an string argument containing the class name you want printed.

```cpp
#include <BMArduino.h>

class MyCleverClass {
  public:
    void clever_function();
};

void MyCleverClass::clever_function() {
  char myCleverString[8] = "Yo!";
  #ifdef BM_DEBUGGING
    CLASS_MSG("MyCleverClass");
    // Let's print the output of a variable.
    Serial.print("The value of myCleverString is: ");
    // note sending the newline
    Serial.println(myCleverString);
  #endif
};

MyCleverClass mineIsClever;

void setup() {
  // use the BM serial implementation
  BM.wait_for_serial(true);

  mineIsClever.clever_function();
}

void loop() {
}
```

The above code will produce something like the following output.

```
@0ms # BM::wait_for_serial() # Hardware Serial begun at 9600 baud.
@6ms # MyCleverClass::clever_function() # The value of myCleverString is: Yo!
```
 
Got a class that you're making a bunch of instances of? Since you can send any string over as the class name, you can use a member of the class itself.

```cpp
#include <BMArduino.h>

class MyEvenMoreCleverClass {
  // For BM.announce(), use BM_CLASS_NAME_SIZE
  char instanceName[BM_CLASS_NAME_SIZE];
  public:
    MyEvenMoreCleverClass::MyEvenMoreCleverClass(char *desiredName);
    void clever_function();
};

MyEvenMoreCleverClass::MyEvenMoreCleverClass(char *desiredName) {
  strcpy(instanceName, desiredName);
}

void MyEvenMoreCleverClass::clever_function() {
  char myCleverString[8] = "Yo!";
  #ifdef BM_DEBUGGING
    // pass our instanceName property to CLASS_MSG()
    CLASS_MSG(instanceName);
    // Let's print the output of a variable.
    Serial.print("The value of myCleverString is: ");
    // note sending the newline
    Serial.println(myCleverString);
  #endif
};

MyEvenMoreCleverClass mineIsEvenMoreCleverA("AwYiss");
MyEvenMoreCleverClass mineIsEvenMoreCleverB("OhYeah");

void setup() {
  // use the BM serial implementation
  BM.wait_for_serial(true);

  mineIsEvenMoreCleverA.clever_function();
  mineIsEvenMoreCleverB.clever_function();
}

void loop() {
}
```

The above code will produce something like the following output.

```
@0ms # BM::wait_for_serial() # Hardware Serial begun at 9600 baud.
@6ms # AwYiss::clever_function() # The value of myCleverString is: Yo!
@80ms # OhYeah::clever_function() # The value of myCleverString is: Yo!
```

### A Note on Compiled Size

In that last example of the "even mnore clever class", the compiled size of the sketch using Arduino IDE 1.6.12 at the moment I wrote this is as follows.

```
Sketch uses 2,372 bytes (7%) of program storage space. Maximum is 32,256 bytes.
Global variables use 495 bytes (24%) of dynamic memory, leaving 1,553 bytes for local variables. Maximum is 2,048 bytes.
```

Turning debugging off yields the following instead.

```
Sketch uses 2,184 bytes (6%) of program storage space. Maximum is 32,256 bytes.
Global variables use 445 bytes (21%) of dynamic memory, leaving 1,603 bytes for local variables. Maximum is 2,048 bytes.
```

Not a huge difference, but noticeable 188 bytes for such a tiny sketch. Be aware that your debug messages do take up space. If you're getting cramped, lean on using the two debug levels to control the output that your *really* need to see or consider not being so verbose.

## Utility Methods

### `announce()`

Opens a line of serial output. It is up to you to end that line by sending newlines or other output. If debug mode is disabled, these methods will produce no output.

Definitions:
```
void announce();
void announce(char *funcName);
void announce(char *className, char *funcName);
```

An example of calling the first variant, without arguments, is below.

```cpp
#include <BMArduino.h>

void setup() {
  // use the BM serial implementation
  BM.wait_for_serial(true);

  BM.announce();
  // note sending the newline
  Serial.println("Sending some serial message.");
}

void loop() {
}
```

The above code will produce something like the following output.

```
@0ms # BM::wait_for_serial() # Hardware Serial begun at 9600 baud.
@6ms # Sending some serial message.
```

The next two variants are mainly used by our debug macros as described in [Using Debug Mode][#using-debug-mode]. Being so, the preferred usage should be via those macros.

Various preprocessor definitions control the output of these methods. You can find and adjust them in [`BMArduino.h`](/BrynM/BMArduino/blob/master/BMArduino.h).

## `info()`

Send some board and C/C++ environment information to serial output.

Definition:
```
void info();
```

Here is an example.

```cpp
#include <BMArduino.h>

void setup() {
  // use the BM serial implementation
  BM.wait_for_serial(true);

  BM.info();
}

void loop() {
}
```

Example output is as follows.

```
@0ms # BM::wait_for_serial() # Hardware Serial begun at 9600 baud.
########################################
# Compiled for ATmega328P (23p avail)
# Nov 20 2016 21:55:25
# libc 20150208 2.0.0
########################################
```

The chip is identified in [`BMBoards.h`](/BrynM/BMArduino/blob/master/BMBoards.h) if possible. the portion that reads "23p avail" refers to the number of pins allocated for [`pin_activate()`](#pin_activate) to use. If I know the amount for a given chip, I'll be putting it in. Otherwise the amount is `BM_MAX_PINS` (`96`).

If debug mode is disabled, this method will produce no output.

### `pin_activate()`

Reserve a pin exclusively. This helps prevent using the same pin twice by mistake as you develop and gives a serial console status as pins are used if debugging is enabled.

Definition:
```
bool pin_activate(int pin, int ioMode);
```

Let's make an example reserving pin 10 for output.

```cpp
#include <BMArduino.h>

void setup() {
  // use the BM serial implementation
  BM.wait_for_serial(true);

  BM.pin_activate(10, OUTPUT);
}

void loop() {
}
```

The above code will produce something like the following output.

```
@0ms # BM::wait_for_serial() # Hardware Serial begun at 9600 baud.
@6ms # BM::pin_activate() # Reserved pin 10 slot 0 (22 remain)
```

One useful aspect of this method is that it returns a boolean value. It can be used to check if a pin has been used, though it will still spit out serial output in doing so.

```cpp
#include <BMArduino.h>

void setup() {
  // use the BM serial implementation
  BM.wait_for_serial(true);

  BM.pin_activate(10, OUTPUT);

  if(!BM.pin_activate(10, OUTPUT)) {
    Serial.println("Could not reserve the pin! It's used!");
  }
}

void loop() {
}
```

The above code will produce something like the following output.

```
@0ms # BM::wait_for_serial() # Hardware Serial begun at 9600 baud.
@6ms # BM::pin_activate() # Reserved pin 10 slot 0 (22 remain)
@72ms # BM::pin_activate() # Failed pin 10 slot 0
Could not reserve the pin! It's used!
```

### `value_is_between()`

Evaluates whether or not a given integer value is between an upper and a lower integer value. Handy for pins that read with a little bit of variance. I needed it for some buttons with some bounce.

Definition:
```
bool value_is_between(int value, int lowerBound, int upperBound);
```

An example of returning `true` and `false` is below.

```cpp
#include <BMArduino.h>

void setup() {
  // use the BM serial implementation
  BM.wait_for_serial(true);

  Serial.print("50 is between 0 and 100 = ");
  Serial.println(BM.value_is_between(50, 0, 100));
  Serial.print("101 is between 0 and 100 = ");
  Serial.println(BM.value_is_between(101, 0, 100));
}

void loop() {
}
```

The above code will produce something like the following output.

```
@0ms # BM::wait_for_serial() # Hardware Serial begun at 9600 baud.
50 is between 0 and 100 = 1
101 is between 0 and 100 = 0
```

### `value_is_near()`

Evaluates whether or not a given integer value is near another integer value with an optional amount of integer variance. Handy for pins that read with a little bit of variance. I needed it for some buttons with some bounce that was produced at different, but predictable, variances.

The default amount of variance is `10` and can be changed with `BM_VALUE_NEAR_DEFAULT_VARIANCE` in [`BMArduino.h`](/BrynM/BMArduino/blob/master/BMArduino.h) though it's often more convenient to call the variant with all three arguments.

Definitions:
```
bool value_is_near(int val, int near);
bool value_is_near(int val, int near, int variance);
```

Here's an example using both variants of this method.

```cpp
#include <BMArduino.h>

void setup() {
  // use the BM serial implementation
  BM.wait_for_serial(true);

  Serial.print("55 is near 50 using the default variance of ");
  Serial.print(BM_VALUE_NEAR_DEFAULT_VARIANCE);
  Serial.print(" = ");
  Serial.println(BM.value_is_near(55, 50));

  Serial.print("45 is near 50 using the default variance of 20 = ");
  Serial.println(BM.value_is_near(45, 50, 20));
}

void loop() {
}
```

The above code will produce something like the following output.

```
@0ms # BM::wait_for_serial() # Hardware Serial begun at 9600 baud.
55 is near 50 using the default variance of 10 = 1
45 is near 50 using the default variance of 20 = 1
```

### `wait_for_serial()`

Wait for either the hardware `Serial` interface to be ready proceed with execution after an elapsed preprocessor set timeout (`BM_SERIAL_TIMEOUT`). This method can optionally also run `Serial.begin()` using a preprocessor set baud rate (`BM_SERIAL_RATE`).

The short of it is I got sick of `while(!Serial)` syntax copy/pasting.

Definitions:
```
void wait_for_serial();
void wait_for_serial(bool startSerial);
```

There are plenty of examples throughout this document.
