/*
 * Copyright 2017 Open Source Robotics Foundation
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
#include <string>
#include <utility>
#include "Utils.hh"

using namespace ignition::math;

/////////////////////////////////////////////////
bool sdf::loadName(sdf::ElementPtr _sdf, std::string &_name)
{
  // Read the name
  std::pair<std::string, bool> namePair = _sdf->Get<std::string>("name", "");

  _name = namePair.first;
  return namePair.second;
}

/////////////////////////////////////////////////
bool sdf::loadPose(sdf::ElementPtr _sdf, Pose3d &_pose,
              std::string &_frame)
{
  sdf::ElementPtr sdf = _sdf;
  if (_sdf->GetName() != "pose")
  {
    if (_sdf->HasElement("pose"))
      sdf = _sdf->GetElement("pose");
    else
      return false;
  }

  // Read the frame. An empty frame implies the parent frame.
  std::pair<std::string, bool> framePair = sdf->Get<std::string>("frame", "");

  // Read the pose value.
  std::pair<Pose3d, bool> posePair =
    sdf->Get<Pose3d>("", Pose3d::Zero);

  // Set output, but only if the return value is true.
  if (posePair.second)
  {
    _pose = posePair.first;
    _frame = framePair.first;
  }

  // The frame attribute is optional, so only return true or false based
  // on the pose element value.
  return posePair.second;
}

//////////////////////////////////////////////////
Pose3d sdf::poseInFrame(const std::string &_src,
    const std::string &_dst, FrameGraph &_graph)
{
  std::cout << "Src[" << _src << "] Dst[" << _dst << "]\n";

  if (_src.empty())
    return Pose3d(INF_D, INF_D, INF_D, INF_D, INF_D, INF_D);

  printf("B\n");
  // Handle the case where the source and destination are the same.
  if (_src == _dst)
    return Matrix4d::Identity.Pose();

  // Get the source vertex.
  const graph::VertexRef_M<Matrix4d> srcVertices = _graph.Vertices(_src);

  // Get all the vertices in the frame graph that match the provided frame.
  // If _dst is empty, then the result of this function (poseInFrame) will
  // be the pose of the _src frame.
  const graph::VertexRef_M<Matrix4d> dstVertices = _graph.Vertices(_dst);

  printf("C\n");
  // There should be only one vertex for the source vertex, and 1 or
  // 0 vertices for the destination vertex.
  if (srcVertices.size() != 1 || dstVertices.size() > 1)
    return Pose3d(INF_D, INF_D, INF_D, INF_D, INF_D, INF_D);
  printf("D\n");

  // Run Dijkstra to find a path from _src to _dst
  std::map<graph::VertexId, graph::CostInfo> result =
    graph::Dijkstra(_graph,
        srcVertices.begin()->first, dstVertices.begin()->first);

  // Dijkstra debug output
  for (auto vv : result)
  {
    std::cout << "DestId[" << vv.first << "] Cost["
      << vv.second.first << "] PrevId[" << vv.second.second
      << "]" << std::endl;
  }

  Matrix4d finalPose = Matrix4d::Identity;
  for (graph::VertexId nextVertex, key = dstVertices.begin()->first;;
       key = nextVertex)
  {
    printf("E\n");
    // Get the next vertex in the path from the destination vertex to the
    // source vertex.
    nextVertex = result.find(key)->second.second;

    printf("Next[%d] Key[%d]\n", nextVertex, key);
    // Are we at the source vertex, which is the end of the line.
    if (nextVertex == key)
    {
      printf("F\n");
      // Compute the final pose and break
      finalPose *= _graph.VertexFromId(key).Data();
      break;
    }

    // Get the edge between nextVertex and key.
    const graph::DirectedEdge<int> &edge =
      _graph.EdgeFromVertices(nextVertex, key);

    // Make sure the edge is valid.
    if (edge.Id() != graph::DirectedEdge<int>::NullEdge.Id())
    {
      // // Debug output:
      // std::cout << "Key[" << key << "] Edge From[" << edge.Head()
      //   << "] To[" << edge.Tail() << "] Data[" << edge.Data() << "]\n";

      // Get the direction of the edge.
      // \todo I think we look at just the Head() and Tail() of the edge and
      // compare those values to key and nextVertex.
      bool inverse = edge.Data() < 0;

      if (inverse)
        finalPose *= _graph.VertexFromId(key).Data().Inverse();
      else if (key != dstVertices.begin()->first)
        finalPose *=  _graph.VertexFromId(key).Data();
    }
    else
    {
      /// \todo This is an error case. Inform the caller somehow.
      std::cerr << "ERRROR!\n";
      break;
    }
  }

  return finalPose.Pose();
}
