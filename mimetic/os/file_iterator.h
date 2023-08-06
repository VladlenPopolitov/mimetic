/***************************************************************************
    copyright            : (C) 2002-2008 by Stefano Barbato
    email                : stefano@codesink.org

    $Id: file_iterator.h,v 1.11 2008-10-27 18:30:42 tat Exp $
 ***************************************************************************/
#ifndef _MIMETIC_OS_FILE_ITERATOR_H_
#define _MIMETIC_OS_FILE_ITERATOR_H_
#include <string>
#include <iterator>

namespace mimetic
{
struct StdFile;

struct ifile_iterator
{
    typedef std::input_iterator_tag iterator_category;
     typedef char value_type;
     typedef std::ptrdiff_t  difference_type;
     typedef char* pointer;
     typedef char& reference;

    ifile_iterator();    
    ifile_iterator(const StdFile* f);
    ifile_iterator(const ifile_iterator&);
    ifile_iterator& operator=(const ifile_iterator&);
    ~ifile_iterator();    
    inline ifile_iterator& operator++();
    inline ifile_iterator operator++(int);
    inline reference operator*();
    inline bool operator!=(const ifile_iterator& right) const;
    inline bool operator==(const ifile_iterator& right) const;
private:
    void cp(const ifile_iterator&);
    void setBufsz();
    enum { defBufsz = 4096 }; // default buffer size(4 missing getpagesize)
    void underflow();
    bool m_eof;
    value_type* m_buf;
    value_type* m_ptr;
    long long m_count;
    const StdFile* m_pFile;
    long long m_read; //bytes read so far
    size_t m_bufsz;
};

inline
ifile_iterator ifile_iterator::operator++(int) // postfix
{
    ifile_iterator cp = *this;
    operator++();
    return cp;
}


inline
ifile_iterator& ifile_iterator::operator++() // prefix
{
    if(--m_count > 0)
        ++m_ptr;
    else
        underflow();
    return *this;
}

inline
ifile_iterator::reference ifile_iterator::operator*()
{
    return *m_ptr;
}

inline
bool ifile_iterator::operator!=(const ifile_iterator& right) const
{
    // always different except when both are EOF
    return !operator==(right);
}


inline
bool ifile_iterator::operator==(const ifile_iterator& right) const
{
    // are equal if both are EOF
    return (m_eof && right.m_eof);
}

}

#endif
