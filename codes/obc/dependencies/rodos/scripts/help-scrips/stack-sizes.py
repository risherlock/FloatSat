#!/usr/bin/python3

import re
import sys

class Node:
  def __init__(self, title, label, edges, size):
    self.title = title
    self.label = label
    self.edges = edges
    self.size = size

class Edge:
  def __init__(self, sourcename, targetname):
    self.sourcename = sourcename
    self.targetname = targetname

files = sys.argv[1:]
nodes = []
edges = []

for f in files:
  file = open(f, "r")
  lines = file.readlines()
  file.close()

  for l in lines[1:]:
    if l[0:4] == "node":
      if " bytes " in l:
        title = re.search("title: \"(.*)\" label:", l).group(1)
        label = re.search("label: \"(.*)\\\\n.*\\\\n", l).group(1)
        size = re.search("([0-9]+) bytes", l).group(1)
        nodes.append(Node(title, label, [], size))
    elif l[0:4] == "edge":
      if "label: " in l:
        sourcename = re.search("sourcename: \"(.*)\" targetname: ", l).group(1)
        targetname = re.search("\" targetname: \"(.*)\" label: ", l).group(1)
        edges.append(Edge(sourcename, targetname))
        
for e in edges:
  for n in nodes:
    if e.sourcename == n.title:
      for m in nodes:
        if m.title == e.targetname:
          n.edges.append(m)
          break
      break

for n in nodes:
  if len(n.edges) > 0:
    max = 0
    for e in n.edges:
      if int(e.size) > max:
        max = int(e.size)
    n.size = int(n.size) + int(max)

# for n in nodes:
#   print(n)
for n in nodes:
  print(n.label + " max stack size: " + str(n.size) + " bytes")
