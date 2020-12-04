#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

using i64 = long long;

bool is_divisible(i64 a, i64 b) {
    return (a / b) * b == a;
}

struct Reactant {
    std::string name;
    i64 amount;
};

struct Reaction {
    Reactant product;
    std::vector<Reactant> reactants;
};

Reaction& find_reaction(std::vector<Reaction>& reactions, std::string const& name) {
    return *std::find_if(reactions.begin(), reactions.end(), [&name](Reaction const& r) { return r.product.name == name; });
}

Reaction const& find_reaction(std::vector<Reaction> const& reactions, std::string const& name) {
    return *std::find_if(reactions.begin(), reactions.end(), [&name](Reaction const& r) { return r.product.name == name; });
}

void read_reaction(std::vector<Reaction>& reactions) {
    Reaction r;
    while(true) {
        i64 a;
        std::string n;
        std::cin >> a >> n;
        if(!std::cin) break;
        if(n[n.size() - 1] == ',') { n.pop_back(); }
        r.reactants.push_back({n, a});
    }

    std::cin.clear();
    std::cin.ignore(200, ' ');

    i64 a;
    std::string n;
    std::cin >> a >> n;
    r.product = {n, a};
    reactions.push_back(r);
}

void add_unused(std::vector<Reactant>& unused_reactants, std::string name, i64 amount) {
    auto iter = std::find_if(unused_reactants.begin(), unused_reactants.end(), [&name](Reactant const& r) { return r.name == name; });
    if(iter != unused_reactants.end()) {
        iter->amount += amount;
    } else {
        unused_reactants.push_back({name, amount});
    }
}

i64 count_reactant(std::vector<Reaction>& reactions, std::vector<Reactant>& unused_reactants, std::string base_reactant, std::string name, i64 required_amount) {
    Reaction& reaction = find_reaction(reactions, name);
    i64 const multiplier = required_amount / reaction.product.amount + !is_divisible(required_amount, reaction.product.amount);
    add_unused(unused_reactants, name, multiplier * reaction.product.amount - required_amount);
    i64 reactant = 0;
    for(Reactant& res: reaction.reactants) {
        if(res.name == base_reactant) {
            reactant += res.amount * multiplier;
        } else {
            reactant += count_reactant(reactions, unused_reactants, base_reactant, res.name, res.amount * multiplier);
        }
    }
    return reactant;
}

i64 unproduce(std::vector<Reaction> const& reactions, std::vector<Reactant>& stock, std::string name, i64 amount) {
    Reaction const& reaction = find_reaction(reactions, name);
    i64 multiplier = amount / reaction.product.amount;
    for(Reactant const& r: reaction.reactants) {
        add_unused(stock, r.name, r.amount * multiplier);
    }
    return multiplier * reaction.product.amount;
}

i64 count_reactant_needed(std::vector<Reaction>& reactions, std::string base_reactant, std::string name, i64 required_amount) {
    std::vector<Reactant> unused_reactants;
    i64 needed = count_reactant(reactions, unused_reactants, base_reactant, name, required_amount);
    std::cout << needed << '\n';
    // Brute-force unproducing
    for(i64 j = 0; j < 1000; ++j) {
        for(i64 i = 0; i < unused_reactants.size(); ++i) {
            if(unused_reactants[i].name != base_reactant) {
                i64 unproduced = unproduce(reactions, unused_reactants, unused_reactants[i].name, unused_reactants[i].amount);
                // std::cout << "unproduced: " << unused_reactants[i].name << " " << unproduced << '\n';
                unused_reactants[i].amount -= unproduced;
            }
        }
    }

    for(Reactant& r: unused_reactants) {
        if(r.name == base_reactant) {
            needed -= r.amount;
            break;
        }
    }

    return needed;
}

int main() {
	std::ios_base::sync_with_stdio(false);

    std::vector<Reaction> reactions;
    while(!std::cin.eof()) {
        read_reaction(reactions);
    }

    i64 ore = count_reactant_needed(reactions, "ORE", "FUEL", 1);
    std::cout << ore;

	return 0;
}
