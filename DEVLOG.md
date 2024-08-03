# DEVLOG

## Daily Journal

### July 6

#### What did I do today?
- Implemented Makefile for `tests/` directory
- Added `camera_test.c` unit test file
- Created axis aligned camera test
- Created arbitrary axis camera test

#### What do I want to do next?
- Verify arbitrary axis camera test
- Add error handling for same points/vectors in `init_camera`

### July 11

#### What did I do today?
- Create error tests for camera (zero up vector, same look from and at)
- Handled error cases in camera init function
- Added check for arbitraty axis camera test

#### What do I want to do next?
- Create tests for ray casting
    - Check FOV is correct
    - Check ray spacing is correct
    - Check ray directions are correct

### July 23

#### What did I do today?
- Check FOV is correct
- Check ray spacing is correct
- In setting up FOV test, I checked directions are correct

#### What do I want to do next?
- Test intersections?
- Start parallelizing code
- Change allocations to be on heap

### July 27

#### What did I do today?
- Install OpenCL on Arch machine
- Detect OpenCL devices present

#### What do I want to do next?
- Create a test kernel module in OpenCL C lang
    - Add a few nums together?
    - Matrix multiplication?
- Figure out how to run kernel on graphics card
    - [Follow steps](https://github.com/KhronosGroup/OpenCL-Guide/blob/main/chapters/how_does_opencl_work.md)

