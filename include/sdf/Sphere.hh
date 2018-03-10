/*
 * Copyright 2018 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#ifndef SDF_SPHERE_HH_
#define SDF_SPHERE_HH_

#include <sdf/Error.hh>
#include <sdf/Element.hh>
#include <sdf/Geometry.hh>

namespace sdf
{
  // Forward declare private data class.
  class SpherePrivate;

  class Sphere : public Geometry
  {
    /// \brief Constructor
    public: Sphere();

    /// \brief Destructor
    public: virtual ~Sphere();

    /// \brief Load the sphere geometry based on a element pointer.
    /// This is *not* the usual entry point. Typical usage of the SDF DOM is
    /// through the Root object.
    /// \param[in] _sdf The SDF Element pointer
    /// \return Errors, which is a vector of Error objects. Each Error includes
    /// an error code and message. An empty vector indicates no error.
    public: Errors Load(ElementPtr _sdf) = 0;

    /// \brief Private data pointer.
    private: SpherePrivate *dataPtr;
  };
}
#endif