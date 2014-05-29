# A c++ client to FastForward &#187;

[![Build Status](https://travis-ci.org/udoprog/libffwd-client.svg?branch=master)](https://travis-ci.org/udoprog/libffwd-client)

A c++ client for the native protobuf protocol of
[ffwd](https://github.com/spotify/ffwd).

## Usage

```c++
# test1.cpp
#include <ffwd.hpp>

const ffwd::client c;
const ffwd::metric foo = ffwd::metric().key("foo");

int main(int argc, char* argv[]) {
  c.send(foo.attribute("id", "foo").value(1));
  c.send(foo.attribute("id", "bar").value(2));
  return 0;
}
```

```bash
$> g++ test1.cpp -lffwd-client -o test1
$> ./test1
```

This project also comes with the `libffwd-client-send` command for
generating events and metrics.

## Installation

libffwd-client currently has to be built and distributed through source.

Dependencies are.

* libffwd-client
 * protobuf
* ffwd-client-send (optional)
 * boost 1.53

```bash
$> cmake .
...
$> make
...
$> sudo make install
```

## Contributing

1. Fork libffwd-client from
   [github](https://github.com/udoprog/libffwd-client) and clone your fork.
2. Hack.
3. Push the branch back to GitHub.
4. Send a pull request to our upstream repo.
