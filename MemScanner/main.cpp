#include <iostream>
#include <algorithm>
#include <exception>
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include "SystemMemory.h"
#include "Scanner.h"
#include "TimeWatch.h"

std::string GetInput()
{
    std::string input;
    std::cout << ">> ";
    std::getline(std::cin, input);
    return input;
}

std::string GetInputLower()
{
    std::string input = GetInput();
    std::transform(input.begin(), input.end(), input.begin(), ::tolower);
    return input;
}
int GetInputInt()
{
    for (;;)
    {
        try
        {
            auto input = GetInput();
            return std::stoi(input);
        }
        catch (const std::logic_error& ex)
        {
            std::cout << ex.what() << '\n';
        }
    }
}

void print_results(Scanner& Scanner, int count)
{
    auto& results = Scanner.Getresults();
    std::cout << "Results: " << results.size() << '\n';
    int i = 0;

    for (const auto adr : results)
    {
        std::cout << static_cast<void*>(adr.address) << " : ";

        switch (Scanner.GetSettings().value_type)
        {
            case ValueType::Int8:
                std::cout << adr.value.int8;
                break;

            case ValueType::Int16:
                std::cout << adr.value.int16;
                break;

            case ValueType::Int32:
                std::cout << adr.value.int32;
                break;

            case ValueType::Int64:
                std::cout << adr.value.int64;
                break;

            case ValueType::Float:
                std::cout << adr.value.float_value;
                break;

            case ValueType::Double:
                std::cout << adr.value.double_value;
                break;

            case ValueType::String:
                break;
        }

        std::cout << '\n';

        if (count != 0 && ++i == count)
        {
            break;
        }
    }
}

void set_alignment(Scanner& scanner)
{
    scanner.GetSettings().alignment = GetInputInt();
}

void set_value_type(Scanner& Scanner)
{
    const std::unordered_map<std::string, ValueType> value_types
    {
        {"int8", ValueType::Int8},
        {"int16", ValueType::Int16},
        {"int32", ValueType::Int32},
        {"int64", ValueType::Int64},
        {"float", ValueType::Float},
        {"double", ValueType::Double},
        {"string", ValueType::String}
    };

    for (;;)
    {
        std::cout << "ValueType: int8, int16, int32, int64, float, double, string\n";
        std::string input = GetInputLower();

        if (input == "!q")
        {
            break;
        }

        auto it = value_types.find(input);

        if (it != value_types.end())
        {
            Scanner.GetSettings().value_type = it->second;
            break;
        }

        std::cout << "Invalid ValueType selection\n";
    }
}

void set_compare_type(Scanner& Scanner)
{
    const std::unordered_map<std::string, CompareType> compare_types
    {
        {"equal", CompareType::Equal},
        {"less", CompareType::Less},
        {"greater", CompareType::Greater},
        {"unknown", CompareType::UnknownType},
        {"increased", CompareType::Increased},
        {"decreased", CompareType::Decreased},
        {"unchanged", CompareType::Unchanged},
        {"changed", CompareType::Changed}
    };

    for (;;)
    {
        std::cout << "CompareType: equal, less, greater, unknown, increased, decreased, unchanged, changed\n";
        std::string input = GetInputLower();

        if (input == "!q")
        {
            break;
        }

        auto it = compare_types.find(input);

        if (it != compare_types.end())
        {
            Scanner.GetSettings().compare_type = it->second;
            break;
        }

        std::cout << "Invalid CompareType selection\n";
    }
}

void scan(Scanner& scanner)
{
    const std::string value_type_names[] = {"int8", "int16", "int32", "int64", "float", "double", "string"};
    const std::string compare_type_names[] = {"equal", "less", "greater", "unknown", "increased", "decreased", "unchanged", "changed"};

    for (;;)
    {
        if (scanner.Size() == 0)
        {
            std::cout << ":: New Scan\n";
        }
        else
        {
            std::cout << ":: Next Scan\n";
        }

        const auto& settings = scanner.GetSettings();
        std::cout << "Settings: valuetype: " << value_type_names[static_cast<int>(settings.value_type)]
                  << ", comparetype: " << compare_type_names[static_cast<int>(settings.compare_type)]
                  << ", alignment: " << settings.alignment << '\n';
        std::string input = GetInput();

        if (input == "!q")
        {
            break;
        }
        else if (input == "!clear")
        {
            scanner.Clear();
            continue;
        }
        else if (input == "!shrink")
        {
            scanner.Shrink();
            continue;
        }

        try
        {
            TimeWatch sw{"Scantime"};
            scanner.Find(input);
        }
        catch (std::exception& ex)
        {
            std::cout << ex.what() << '\n';
            continue;
        }

        print_results(scanner, 10);
    }
}

void menu(Scanner& scanner)
{
    const std::unordered_map<std::string, void(*)(Scanner&)> menu_items
    {
        {"alignment", set_alignment},
        {"valuetype", set_value_type},
        {"comparetype", set_compare_type},
        {"scan", scan}
    };

    for (;;)
    {
        std::cout << "Menu: alignment, valuetype, comparetype, scan\n";
        std::string input = GetInputLower();

        if (input == "!q")
        {
            break;
        }

        auto it = menu_items.find(input);

        if (it != menu_items.end())
        {
            it->second(scanner);
            continue;
        }

        std::cout << "Invalid menu selection\n";
    }
}

int main()
{
    for (;;)
    {
        try
        {
            size_t pid;
            std::cout << "Input process id\n";
            pid = GetInputInt();
            Scanner Scanner{pid};
            menu(Scanner);
        }
        catch (std::runtime_error& ex)
        {
            std::cout << ex.what() << '\n';
            continue;
        }

        break;
    }

    return 0;
}
