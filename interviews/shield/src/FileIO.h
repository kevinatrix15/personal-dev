#pragma once

#include "Map.h"
#include "Point.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

constexpr char DELIM = ' ';

class ConfigSpaceIO
{
  public:
  static void write(const ConfigurationSpace& configSpace,
                    const std::filesystem::path& filePath)
  {
    // create directory structure
    std::filesystem::create_directories(filePath.parent_path());

    // create file ofstream
    std::ofstream outStream(filePath.string(), std::ios::out);
    if (!outStream) {
      throw std::runtime_error("Failed to open file for writing: " + filePath.string());
    }

    // write header info
    outStream << configSpace.robotRadius() << std::endl;
    outStream << configSpace.numX() << std::endl;
    outStream << configSpace.numY() << std::endl;

    // write data to file using object's << operator
    outStream << configSpace;
  }

  static ConfigurationSpace read(const std::filesystem::path& filePath)
  {
    // check file exists and can be read
    if (!std::filesystem::exists(filePath)) {
      throw std::runtime_error("File not found for reading: " + filePath.string());
    }

    std::ifstream inStream(filePath, std::ios::in);
    if (!inStream) {
      throw std::runtime_error("Failed to open file for reading: " + filePath.string());
    }

    try {
      std::string line;
      // assign robot radius
      getline(inStream, line);
      const size_t robotRadius = static_cast<size_t>(std::stoi(line));

      // determine nx and ny from file while reading cell values
      getline(inStream, line);
      const size_t nx = static_cast<size_t>(std::stoi(line));
      getline(inStream, line);
      const size_t ny = static_cast<size_t>(std::stoi(line));

      std::string entry;
      std::vector<cell_state> cellData;
      while (getline(inStream, line)) {
        std::vector<cell_state> row;
        std::stringstream s(line);
        while(getline(s, entry, DELIM)) {
          row.emplace_back(static_cast<cell_state>(std::stoi(entry)));
        }
        // append the row to the end of the cellData
        std::move(row.begin(), row.end(), std::back_inserter(cellData));
        row.clear();
      }

      // ensure the data is properly sized
      assert(nx * ny == cellData.size());

      // construct and return the ConfigurationSpace
      DataMap<cell_state> dataMap(std::make_pair(nx, ny), cellData);
      return ConfigurationSpace(dataMap, robotRadius);
    } 
    catch (const std::exception& e) {
      std::cerr << "Error reading configuration space data: " << e.what() << std::endl;
      throw(e);
    }
  }
};

class SolutionPathIO
{
  public:
  static void write(const std::vector<Point>& solutionPath,
                    const std::filesystem::path& filePath)
  {
    // create directory structure
    std::filesystem::create_directories(filePath.parent_path());

    // create file ofstream
    std::ofstream outStream(filePath.string(), std::ios::out);
    if (!outStream) {
      throw std::runtime_error("Failed to open file for writing: " + filePath.string());
    }

    // write data to file using object's << operator
    for (const auto& pt : solutionPath) {
      outStream << pt.x() << DELIM << pt.y() << std::endl;
    }
  }

  static std::vector<Point> read(const std::filesystem::path& filePath)
  {
    // check file exists and can be read
    if (!std::filesystem::exists(filePath)) {
      throw std::runtime_error("File not found for reading: " + filePath.string());
    }

    std::ifstream inStream(filePath, std::ios::in);
    if (!inStream) {
      throw std::runtime_error("Failed to open file for reading: " + filePath.string());
    }

    try {
      std::string line;
      std::string entry;
      std::vector<Point> path;
      while (getline(inStream, line)) {
        std::stringstream s(line);
        
        // read the x and y components and create the point
        Point p;
        getline(s, entry, DELIM);
        p.x() = static_cast<size_t>(std::stoi(entry));
        getline(s, entry, DELIM);
        p.y() = static_cast<size_t>(std::stoi(entry));

        // add the point to the path
        path.emplace_back(p);
      }
      return path;
    }
    catch (const std::exception& e) {
      std::cerr << "Error reading configuration space data: " << e.what() << std::endl;
      throw(e);
    }
  }
};