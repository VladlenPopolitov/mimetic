/***************************************************************************
    copyright            : (C) 2002-2008 by Stefano Barbato
    email                : stefano@codesink.org

    $Id: strutils.cxx,v 1.3 2008-10-07 11:06:26 tat Exp $
 ***************************************************************************/
#include <mimetic/strutils.h>
#include <cassert>

namespace mimetic
{
using namespace std;

const string nullstring;


string canonical(const string& s, bool no_ws)
{
    if(s.empty())
        return s;
    string input = s;
    // removes leading spaces
    size_t idx = 0;
    while(input[idx] == ' ')
        idx++;
    if(idx)
        input.erase(0, idx);

    if(input.empty())
        return input;

    // removes trailing spaces
    idx = input.length() - 1;
    while(input[idx] == ' ')
        idx--;
    input.erase(1 + idx, string::npos);
    // removes rfc822 comments and non-required spaces
    bool in_dquote = false, has_brack = false;
    size_t in_par = 0, in_brack = 0, par_last = 0;
    assert(input.length() > 0); // empty input processed above
    for (auto t = input.length() - 1; ; --t)
    {
        if (input[t] == '"') {
            in_dquote = !in_dquote;
        }
        else if (in_dquote) {
            continue;
        }
        else if (input[t] == '<') {
            in_brack--;
        }
        else if (input[t] == '>') {
            has_brack = true;
            in_brack++;
        }
        else if (input[t] == ')') {
            in_par++;
            if (in_par == 1)
                par_last = t;
        }
        else if (input[t] == '(') {
            in_par--;
            if (in_par == 0)
            {
                input.erase(t, 1 + par_last - t);
                // comments will be replaced with a space in
                // !no_ws
                if (!no_ws)
                    input.insert(t, " ");
            }
        }
        else if (no_ws && input[t] == ' ' && !in_par && !has_brack) {
            input.erase(t, 1);
        }
        if (t == 0) break;
    }
    
    return input;
}

}
