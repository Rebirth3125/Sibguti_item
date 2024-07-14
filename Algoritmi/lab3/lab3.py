import numpy as np
import csv


def read_data(filename):
  with open(filename, "r") as fp:
    reader = csv.reader(fp)
    data = list(reader)
  supply = list(int(row[-1]) for row in data[0:-1])
  demand = list(map(int, data[-1]))
  costs = np.array(list(list(map(int, row[0:-1])) for row in data[0:-1]))
  return supply, demand, costs


def northwest_corner_method(supply, demand, costs):
  m, n = len(supply), len(demand)
  allocation = np.zeros((m, n))

  i, j = 0, 0
  while i < m and j < n:
    quantity = min(supply[i], demand[j])
    allocation[i, j] = quantity
    supply[i] -= quantity
    demand[j] -= quantity
    if supply[i] == 0:
      i += 1
    if demand[j] == 0:
      j += 1
    print(allocation)

    print()
  return allocation


def transport_problem(supply, demand, costs):
  m, n = len(supply), len(demand)
  closed = True

  if sum(supply) != sum(demand):
    closed = False
    if sum(supply) > sum(demand):
      demand.append(sum(supply) - sum(demand))
      costs = np.hstack((costs, np.zeros((m, 1))))
    else:
      supply.append(sum(demand) - sum(supply))
      costs = np.vstack((costs, np.zeros((1, n))))

  allocation = northwest_corner_method(supply, demand, costs)
  total_cost = np.sum(allocation * costs)

  print("Optimal allocation:")
  for i in allocation:
    for j in i:
      if j == 0 :
        print(" - ",end=' ')
      else:
        print(j, end=' ')
    print()
 # print(allocation)
  print()
  print("Total cost: ", total_cost)

  return allocation, total_cost


supply, demand, costs = read_data("data.csv")
transport_problem(supply, demand, costs)
