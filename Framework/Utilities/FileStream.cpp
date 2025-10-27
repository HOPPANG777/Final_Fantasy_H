#include "Framework.h"
#include "FileStream.h"

FileStream::FileStream(std::string path, UINT flags)
    : flag(flags)
{
    int iosFlags = std::ios::binary;

    iosFlags |= flags && FILE_STREAM_WRITE
        ? std::ios::out : 0;
    iosFlags |= flags && FILE_STREAM_READ
        ? std::ios::in : 0;

    if (flags & FILE_STREAM_WRITE)
    {
        out.open(path, iosFlags);
        if (out.fail())
        {
            out.flush();
            out.close();
            assert(false);
        }
    }
    else if (flags & FILE_STREAM_READ)
    {
        in.open(path, iosFlags);
        if (in.fail())
        {
            in.clear();
            in.close();
            assert(false);
        }
    }
}

FileStream::~FileStream()
{
    if (flag & FILE_STREAM_WRITE)
    {
        out.flush();
        out.close();
    }
    else if (flag & FILE_STREAM_READ)
    {
        in.clear();
        in.close();
    }

}
