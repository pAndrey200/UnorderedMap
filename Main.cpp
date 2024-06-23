#include <iostream>
#include <initializer_list>
#include <exception>
#include "Container.h"
#include "Iterator.h"
#include "Algorithms.h"

struct Goods
{
	std::string m_name;
	std::string m_manufacturer;
	std::string m_warehouse_address;
	double m_weight;

	Goods(): m_name("\0"), m_manufacturer("\0"), m_warehouse_address("\0"), m_weight(0.0) {}

	Goods(
	    const std::string& name,
	    const std::string& manufacturer,
	    const std::string& warehouse_address,
	    double weight)
		: m_name(name)
		, m_manufacturer(manufacturer)
		, m_warehouse_address(warehouse_address)
		, m_weight(weight)
	{}


	friend std::ostream& operator << (std::ostream& out, const Goods& goods)
	{
		out << "Name: " << goods.m_name << std::endl;
		out << "Manufacturer: " << goods.m_manufacturer << std::endl;
		out << "Warehouse address: " << goods.m_warehouse_address << std::endl;
		out << "Weight: " << goods.m_weight << std::endl;
		return out;
	}
};

int main()
{
	try
	{
		auto pt = [](const auto& x)
		{
			std::cout <<"key: "<< x.first <<std::endl << "data:\n" << x.second;
			return x;
		};
		std::cout << std::endl << "TASK 2" << std::endl;
		UnorderedMap<int, Goods> cont1 = { {1,{ "bread","Kolomenskiy","115230",200.55 }},{2,{ "bread","Kolomenskiy","115230",200.55 }}};
		forEach(cont1.begin(), cont1.end(), pt);
		std::cout << std::endl << "TASK 3" << std::endl;
		Goods g1{"cheese", "Lamber", "131256", 150 };
		Goods g2{"protein bar","Bombbar","145634", 70 };
		Goods g3{"Pepsi","Pepsi-Co Company","152367", 450 };
		UnorderedMap<int, Goods> cont2;

		cont2.insert(std::make_pair(3, g1));
		cont2.insert(std::make_pair(4, g2));
		cont2.insert(std::make_pair(5, g3));
		forEach(cont2.begin(), cont2.end(), pt);
		std::cout << std::endl << "TASK 4" << std::endl;
		cont1.insert(cont2.begin(), cont2.end());
		forEach(cont1.begin(), cont1.end(), pt);
		std::cout << std::endl << "TASK 5" << std::endl;
		UnorderedMap<int, Goods>::iterator res = findIf(cont1.begin(), cont1.end(), [](const auto& f)
			{
				return f.second.m_weight == 150;
			});
		std::cout << "key: " << (*res).first << std::endl << "data:\n" << (*res).second;
		std::cout << std::endl << "TASK 6" << std::endl;
		res->second.m_warehouse_address = "134256";
		forEach(cont1.begin(), cont1.end(), pt);
	}
	catch (const std::exception& err)
	{
		std::cout << err.what();
	}
	return 0;
}
