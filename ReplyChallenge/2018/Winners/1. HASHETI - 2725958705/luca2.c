#include <bits/stdc++.h>

using namespace std;

int V, S, C, P;

typedef struct {
  string name;
  int packages;
  float cost;
  vector<int> unit;
  vector<int> latency;
} region;

vector<string> services;
vector<string> countries;

vector<vector<region>> providerRegions;

vector<long long> availability;

typedef struct {
  int penalty;
  int country;
  vector<int> requirements;
} prj;

vector<prj> projects;

vector<tuple<int, int, int>> solutions;

double RegionPenalty(const prj &project, const region &region, int num_uses) {
  int total_requirements = 0;
  for (auto v : project.requirements)
    total_requirements += v;
  if (!total_requirements)
    throw std::runtime_error("ciao");
  int future_requirements = 0;
  int overassign = 0;
  for (size_t i = 0; i < project.requirements.size(); i++) {
    if (project.requirements[i] > region.unit[i])
      future_requirements += project.requirements[i] - region.unit[i];
    else {
      overassign += region.unit[i] - project.requirements[i];
    }
  }
  double perc = 1 - 0.3 * future_requirements / total_requirements;
  if (perc < 1e-6)
    return std::numeric_limits<double>::max();
  return region.cost * region.latency[project.country] / perc +
         1000 * num_uses + overassign * 10;
}

int nextInt() {
  int x;
  cin >> x;
  return x;
}

int main() {
  V = nextInt();
  S = nextInt();
  C = nextInt();
  P = nextInt();
  availability.resize(P);

  for (int i = 0; i < S; i++) {
    string tmp;
    cin >> tmp;
    services.push_back(tmp);
  }

  for (int i = 0; i < C; i++) {
    string tmp;
    cin >> tmp;
    countries.push_back(tmp);
  }

  for (int i = 0; i < V; i++) {
    string provider;
    int provReg;

    cin >> provider;
    cin >> provReg;

    providerRegions.push_back(vector<region>());

    for (int j = 0; j < provReg; j++) {
      region reg;

      cin >> reg.name;
      cin >> reg.packages;
      cin >> reg.cost;

      for (int k = 0; k < S; k++) {
        int tmp;
        cin >> tmp;
        availability[reg.unit.size()] += reg.packages * tmp;
        reg.unit.push_back(tmp);
      }

      for (int k = 0; k < C; k++) {
        int tmp;
        cin >> tmp;
        reg.latency.push_back(tmp);
      }

      providerRegions[i].push_back(reg);
    }
  }

  for (int i = 0; i < P; i++) {
    string country;
    int pen;
    cin >> pen;
    cin >> country;
    vector<int> units;
    for (int j = 0; j < S; j++) {
      int tmp;
      cin >> tmp;
      units.push_back(tmp);
    }

    prj proj;
    proj.penalty = pen;
    proj.country = std::find(countries.begin(), countries.end(), country) -
                   countries.begin();
    proj.requirements = units;
    projects.push_back(proj);
  }

  std::vector<std::string> outputs(P);

  std::vector<int> expected_cost;
  expected_cost.resize(P);
  std::vector<int> projs;
  for (size_t i = 0; i < P; i++)
    projs.push_back(i);
  for (size_t i = 0; i < P; i++) {
    for (size_t j = 0; j < projects[i].requirements.size(); j++) {
      expected_cost[i] += projects[i].requirements[j];
    }
  }
  std::sort(projs.begin(), projs.end(),
            [&](int a, int b) { return expected_cost[a] < expected_cost[b]; });
  // RegionPenalty(const prj &project, const region &region, int num_uses)
  std::vector<std::map<pair<int, int>, int>> used_vec(P);
  int failed = 0;
  int done = 0;
  for (int i : projs) {
    std::cerr << "Failed: " << failed << " done: " << done << "\r";
    auto &used = used_vec[i];

    bool impossible = false;
    for (size_t j = 0; j < projects[i].requirements.size(); j++) {
      if (availability[j] < projects[i].requirements[j])
        impossible = true;
    }
    if (impossible)
      continue;
    std::multimap<double, pair<int, int>> costs;
    while (true) {
      pair<int, int> min = make_pair(0, 0);
      double minCost = std::numeric_limits<double>::max();
      for (int j = 0; j < providerRegions.size(); j++) {
        for (int k = 0; k < providerRegions[j].size(); k++) {
          int num_uses = 0;
          if (used.count({j, k}))
            num_uses = used.at({j, k});
          if (!providerRegions[j][k].packages)
            continue;
          double pen =
              RegionPenalty(projects[i], providerRegions[j][k], num_uses);
          if (pen < minCost) {
            min = {j, k};
            minCost = pen;
          }
        }
      }
      // unsatisfiable project
      if (minCost == std::numeric_limits<double>::max()) {
        failed++;
        break;
      }
      used[min]++;
      region &chosen = providerRegions[min.first][min.second];
      bool is_done = true;
      for (size_t j = 0; j < projects[i].requirements.size(); j++) {
        availability[j] -= chosen.unit[j];
        if (projects[i].requirements[j] <= chosen.unit[j]) {
          projects[i].requirements[j] = 0;
          continue;
        }
        projects[i].requirements[j] -= chosen.unit[j];
        is_done = false;
      }
      chosen.packages--;
      if (is_done)
        break;
    }
    done++;
  }
  for (int i : projs) {
    std::cerr << "Failed: " << failed << " done: " << done << "\r";
    auto &used = used_vec[i];

    bool ok = true;
    for (size_t j = 0; j < projects[i].requirements.size(); j++) {
      if (availability[j] < projects[i].requirements[j])
        ok = false;
    }
    if (!ok) {
      std::multimap<double, pair<int, int>> costs;
      while (true) {
        pair<int, int> min = make_pair(0, 0);
        double minCost = std::numeric_limits<double>::max();
        for (int j = 0; j < providerRegions.size(); j++) {
          for (int k = 0; k < providerRegions[j].size(); k++) {
            int num_uses = 0;
            if (used.count({j, k}))
              num_uses = used.at({j, k});
            if (!providerRegions[j][k].packages)
              continue;
            double pen =
                RegionPenalty(projects[i], providerRegions[j][k], num_uses);
            if (pen < minCost) {
              min = {j, k};
              minCost = pen;
            }
          }
        }
        // unsatisfiable project
        if (minCost == std::numeric_limits<double>::max()) {
          failed++;
          break;
        }
        used[min]++;
        region &chosen = providerRegions[min.first][min.second];
        bool is_done = true;
        for (size_t j = 0; j < projects[i].requirements.size(); j++) {
          availability[j] -= chosen.unit[j];
          if (projects[i].requirements[j] <= chosen.unit[j]) {
            projects[i].requirements[j] = 0;
            continue;
          }
          projects[i].requirements[j] -= chosen.unit[j];
          is_done = false;
        }
        chosen.packages--;
        if (is_done)
          break;
      }
      done++;
    }
    for (auto v : used) {
      outputs[i] += std::to_string(v.first.first) + " " +
                    std::to_string(v.first.second) + " " +
                    std::to_string(v.second) + " ";
    }
  }
  for (const string &s : outputs) {
    std::cout << s << std::endl;
  }

  std::cerr << std::endl;
  return 0;
}
