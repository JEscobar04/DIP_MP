# Parallel Digital Image Processing

Apply digital image processing transformations on PPM files.

## Program Files

- [dip_seq.c](./src/dip_seq.c)
- [dip_mp_for.c](./src/dip_mp_for.c)
- [dip_mp_tasks.c](./src/dip_mp_tasks.c)
- [Image.c](./src/Image.c)
- [DIPs.c](./src/DIPs.c)
- [FileIO.c](./src/FileIO.c)
- [Test.c](./src/Test.c)
- [Image.h](./src/Image.h)
- [DIPs.h](./src/DIPs.h)
- [FileIO.h](./src/FileIO.h)
- [Test.h](./src/Test.h)
- [Constants.h](./src/Constants.h)
- [utils.c](./src/utils.c)

## Build Instructions

```sh
cd src/
make
```

## Run Instructions

### Prepare Workload

Run the following script to download a sample image and create `N` copies.

```sh
./download.sh [N]
```

### Sequential Version

A sequential implementation of the project for gathering baseline runtimes.

```sh
./dip_seq [FILE_NAME...]
```

### Parallel Version (FOR)

A parallel implementation that distributes files across threads using OpenMP's loop worksharing construct.

```sh
./dip_mp_for [FILE_NAME...]
```

### Parallel Version (TASKS)

A parallel implementation that distributes work across threads using OpenMP's task worksharing construct.

```sh
./dip_mp_tasks [FILE_NAME...]
```
