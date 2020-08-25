/*
 * ------------------------------------------------------------------------------------------------------------
 * SPDX-License-Identifier: LGPL-2.1-only
 *
 * Copyright (c) 2018-2020 Lawrence Livermore National Security LLC
 * Copyright (c) 2018-2020 The Board of Trustees of the Leland Stanford Junior University
 * Copyright (c) 2018-2020 Total, S.A
 * Copyright (c) 2020-     GEOSX Contributors
 * All rights reserved
 *
 * See top level LICENSE, COPYRIGHT, CONTRIBUTORS, NOTICE, and ACKNOWLEDGEMENTS files for details.
 * ------------------------------------------------------------------------------------------------------------
 */

// Header include
#include "StringUtils.hpp"

// Std library includes
#include <algorithm>

namespace PVTPackage {
void StringUtils::Trim(std::string& str)
{
  if (str.size() > 0)
  {
    int first = int(str.find_first_not_of(" \r"));
    int last = int(str.find_last_not_of(" \r"));
    if (first<0)
    {
      str.resize(0);
    }
    else
    {
      str = str.substr(first, (last - first + 1));
    }

  }
}


bool StringUtils::RemoveStringAndFollowingContentFromLine(std::string ToBeRemoved, std::string& line)
{
  // does the line have a semi-colon?
  std::size_t pos = line.find(ToBeRemoved);
  bool res = false;
  if (pos != std::string::npos)
  {
    // remove the character and everything afterwards
    line = line.substr(0, pos);
    res = true;
  }
  return res;
}

std::string StringUtils::RemoveString(std::string ToBeRemoved, std::string data)
{
  std::size_t pos = data.find(ToBeRemoved);
  
  if (pos != std::string::npos)
  {
    std::string line;
    std::size_t L1 = ToBeRemoved.size();
    std::size_t L2 = data.size();
    line = data.substr(pos + L1, L2 - 1);
    return line;
  }
  else
  {
    return data;
  }
  
}


void StringUtils::RemoveTab(std::vector<std::string>& v)
{

  for (unsigned int i = 0; i < v.size(); i++)
  {
    std::replace(v[i].begin(), v[i].end(), '\t', ' ');
  }

  
}

void StringUtils::RemoveEndOfLine(std::vector<std::string>& v)
{

  for (unsigned int i = 0; i < v.size(); i++)
  {
    RemoveEndOfLine(v[i]);
  }

}

void StringUtils::RemoveTab(std::string& v)
{

  std::replace(v.begin(), v.end(), '\t', ' ');

}

void StringUtils::RemoveEndOfLine(std::string& v)
{

  std::replace(v.begin(), v.end(), '\r', ' ');
  std::replace(v.begin(), v.end(), '\n', ' ');
}

bool StringUtils::ExtendDefault(int n, std::string& v)
{

  std::stringstream iss(v);
  std::string sub;
  int cpt = 0;
  while (std::getline(iss, sub, ' '))  //Count number of elements in string
  {
    cpt++;
  }

  v = iss.str();

  for (int i=cpt ; i<n ; i++)
  { 
    v = v +  " DEFAULT";
  }

  RemoveExtraSpaces(v);

  return cpt <= n;


}

void StringUtils::RemoveExtraSpaces(std::string& v)
{

  v.erase(std::unique(v.begin(), v.end(),
    [](char a, char b) { return a == ' ' && b == ' '; }), v.end());

}


std::string StringUtils::GetNthWord(int n, std::string data)
{

  std::size_t found = data.find(" ");

  if ((found == std::string::npos) && (n > 1))
    return "";

  int i = n-1;
  while (i > 0) //Remove n-1 first words
  {
    auto pos = data.find_first_of(' ');
    if (pos == std::string::npos) data = "";
    data = data.substr(pos + 1);
    i--;
  }

  return data.substr(0, data.find(' '));

}
} // namespace
