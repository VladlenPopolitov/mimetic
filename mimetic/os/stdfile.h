/***************************************************************************
    copyright            : (C) 2002-2008 by Stefano Barbato
    email                : stefano@codesink.org

    $Id: stdfile.h,v 1.9 2008-10-07 11:06:26 tat Exp $
 ***************************************************************************/
#ifndef _MIMETIC_OS_STDFILE_H
#define _MIMETIC_OS_STDFILE_H
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstdio>
#include <string>
#include <iterator>
#include <mimetic/os/fileop.h>
#include <mimetic/os/file_iterator.h>

namespace mimetic
{

// File class
struct StdFile: public FileOp
{
    typedef ifile_iterator iterator;
    typedef ifile_iterator const_iterator;
    StdFile();
#ifdef O_BINARY
    StdFile(const std::string&, int mode = (O_RDONLY | O_BINARY));
#else 
    StdFile(const std::string&, int mode = O_RDONLY);
#endif
    ~StdFile();
    operator bool() const;
#ifdef O_BINARY
    void open(const std::string&, int mode = (O_RDONLY | O_BINARY));
#else
    void open(const std::string&, int mode = O_RDONLY);
#endif
    void close();
    size_t read(char*, size_t) const;

    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;
protected:
    void open(int flags);
    bool stat();

    std::string m_fqn;
    bool m_stated;
    struct stat m_st;
    int m_fd;
};

}


#endif

