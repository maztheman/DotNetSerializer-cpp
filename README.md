
# Project Summary

This project attempts to deserialize a .NET object.  
I created this project because I could not find any other library that could do what I wanted.
This is a toy project.

# ChangeLog

## Alpha 0.1.0

---

- Simplfy the code
- Added more primitive types (unsigned ints, signed byte)
- Fixed multidimensional code to actually read the proper amount, logic might not be 100% great
- Fixed all array types to do the right thing
  - Fixed so the arrays use the array filler schema type properly (8 bit and 32bit)

## Alpha 0.0.2

---

- Use cmake to build now
- Should work with any compiler ?
- Code is seprated into library and a example app program


## Alpha 0.0.1

---

### New stuff:

  * User Classes
  * System Classes
  * Generic Classes
  * Arrays
  * Most primatives (IntXX, Double, DateTime, string)
  * Multiple Serialized Assemblies

### Fixed:

- member references are hooked up properly now.
  
# TODO:

---

- Add More types (some primatives not supported, but easily added)
- Try to reduce allocs

