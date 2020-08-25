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

#pragma once

#include <string>

namespace PVTPackage {
class File
{

public:
  explicit File(std::string fileName);
  ~File();

  std::string getFullName() const { return m_FullName; }
  std::string getDirectory() const { return m_Directory; }
  std::string getShortName() const { return m_ShortName; }
  std::string getExtension() const { return m_Extension; }
  std::string getNameWithoutExtension() const { return m_NameWithoutExtension; }
  
private:

  std::string m_FullName;
  std::string m_Directory;
  std::string m_ShortName;
  std::string m_Extension;
  std::string m_NameWithoutExtension;

};
} // namespace
