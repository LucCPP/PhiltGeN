#pragma once

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <functional>

namespace Philtgen
{
    struct PgnData
    {
        std::string site_;
        std::string white_;
        std::string white_title_;
        std::string black_;
        std::string black_title_;
        std::string result_;
        std::string date_;

        std::string termination_;
        std::string event_;
        std::string round_;

        std::string eco_;
        std::string opening_;
        std::string time_control_;

        int year_{ -9'999'999 };
        int month_{ -9'999'999 };
        int day_{ -9'999'999 };

        int white_elo_{ -9'999'999 };
        int black_elo_{ -9'999'999 };
    };

    enum class MissingInfo
    {
        fail,
        pass,
    };

    class Filter
    {
        private:
        std::function<bool(const PgnData& data, const Filter& f)> f_1;
        MissingInfo missing_info_rule_{ MissingInfo::fail };

        bool evaluate(const PgnData& data) const
        {
            return f_1(data, *this);
        }

        friend class Game;

        public:
        Filter(std::function<bool(const PgnData& data, const Filter& f)> f_2) : f_1(f_2) {}

        void missing_info_rule(MissingInfo rule)
        {
            missing_info_rule_ = rule;
        }

        friend Filter operator&&(const Filter& left, const Filter& right)
        {
            return Filter([left, right](const PgnData& data, const Filter& f)
            {
                return left.f_1(data, f) && right.f_1(data, f);
            });
        }

        friend Filter operator||(const Filter& left, const Filter& right)
        {
            return Filter([left, right](const PgnData& data, const Filter& f)
            {
                return left.f_1(data, f) || right.f_1(data, f);
            });
        }

        friend Filter operator!(const Filter& filter)
        {
            return Filter([filter](const PgnData& data, const Filter& f)
            {
                return !filter.f_1(data, f);
            });
        }
    };

    class Game
    {
        private:
        PgnData data_;
        static constexpr int any{ -9'999'999 };

        public:
        Filter site(const std::string& link) const
        {
            return Filter([link](const PgnData& data, const Filter& f)
            {
                if (data.site_.empty())
                {
                    if (f.missing_info_rule_ == MissingInfo::fail)
                    {
                        return false;
                    }
                    else if (f.missing_info_rule_ == MissingInfo::pass)
                    {
                        return true;
                    }
                }

                return data.site_.starts_with(link);
            });
        }

        Filter white(const std::string& name) const
        {
            return Filter([name](const PgnData& data, const Filter& f)
            {
                if (data.white_.empty())
                {
                    if (f.missing_info_rule_ == MissingInfo::fail)
                    {
                        return false;
                    }
                    else if (f.missing_info_rule_ == MissingInfo::pass)
                    {
                        return true;
                    }
                }

                return data.white_ == name;
            });
        }

        Filter white_title(const std::string& title) const
        {
            return Filter([title](const PgnData& data, const Filter& f)
            {
                if (data.white_title_.empty())
                {
                    if (f.missing_info_rule_ == MissingInfo::fail)
                    {
                        return false;
                    }
                    else if (f.missing_info_rule_ == MissingInfo::pass)
                    {
                        return true;
                    }
                }

                return data.white_title_ == title;
            });
        }

        Filter black(const std::string& name) const
        {
            return Filter([name](const PgnData& data, const Filter& f)
            {
                if (data.black_.empty())
                {
                    if (f.missing_info_rule_ == MissingInfo::fail)
                    {
                        return false;
                    }
                    else if (f.missing_info_rule_ == MissingInfo::pass)
                    {
                        return true;
                    }
                }

                return data.black_ == name;
            });
        }

        Filter black_title(const std::string& title) const
        {
            return Filter([title](const PgnData& data, const Filter& f)
            {
                if (data.black_title_.empty())
                {
                    if (f.missing_info_rule_ == MissingInfo::fail)
                    {
                        return false;
                    }
                    else if (f.missing_info_rule_ == MissingInfo::pass)
                    {
                        return true;
                    }
                }

                return data.black_title_ == title;
            });
        }

        Filter termination(const std::string& end) const
        {
            return Filter([end](const PgnData& data, const Filter& f)
            {
                if (data.termination_.empty())
                {
                    if (f.missing_info_rule_ == MissingInfo::fail)
                    {
                        return false;
                    }
                    else if (f.missing_info_rule_ == MissingInfo::pass)
                    {
                        return true;
                    }
                }

                return data.termination_ == end;
            });
        }

        Filter event(const std::string& name) const
        {
            return Filter([name](const PgnData& data, const Filter& f)
            {
                if (data.event_.empty())
                {
                    if (f.missing_info_rule_ == MissingInfo::fail)
                    {
                        return false;
                    }
                    else if (f.missing_info_rule_ == MissingInfo::pass)
                    {
                        return true;
                    }
                }

                return data.event_ == name;
            });
        }

        Filter round(const std::string& name) const
        {
            return Filter([name](const PgnData& data, const Filter& f)
            {
                if (data.round_.empty())
                {
                    if (f.missing_info_rule_ == MissingInfo::fail)
                    {
                        return false;
                    }
                    else if (f.missing_info_rule_ == MissingInfo::pass)
                    {
                        return true;
                    }
                }

                return data.round_ == name;
            });
        }

        Filter result(const std::string& outcome) const
        {
            return Filter([outcome](const PgnData& data, const Filter& f)
            {
                if (data.result_.empty())
                {
                    if (f.missing_info_rule_ == MissingInfo::fail)
                    {
                        return false;
                    }
                    else if (f.missing_info_rule_ == MissingInfo::pass)
                    {
                        return true;
                    }
                }

                return data.result_ == outcome;
            });
        }

        Filter year(int time) const
        {
            return Filter([time](const PgnData& data, const Filter& f)
            {
                if (data.year_ == any)
                {
                    if (f.missing_info_rule_ == MissingInfo::fail)
                    {
                        return false;
                    }
                    else if (f.missing_info_rule_ == MissingInfo::pass)
                    {
                        return true;
                    }
                }

                return data.year_ == time;
            });
        }

        Filter month(int time) const
        {
            return Filter([time](const PgnData& data, const Filter& f)
            {
                if (data.month_ == any)
                {
                    if (f.missing_info_rule_ == MissingInfo::fail)
                    {
                        return false;
                    }
                    else if (f.missing_info_rule_ == MissingInfo::pass)
                    {
                        return true;
                    }
                }

                return data.month_ == time;
            });
        }

        Filter day(int time) const
        {
            return Filter([time](const PgnData& data, const Filter& f)
            {
                if (data.day_ == any)
                {
                    if (f.missing_info_rule_ == MissingInfo::fail)
                    {
                        return false;
                    }
                    else if (f.missing_info_rule_ == MissingInfo::pass)
                    {
                        return true;
                    }
                }

                return data.day_ == time;
            });
        }

        Filter min_white_elo(int elo) const
        {
            return Filter([elo](const PgnData& data, const Filter& f)
            {
                if (data.white_elo_ == any)
                {
                    if (f.missing_info_rule_ == MissingInfo::fail)
                    {
                        return false;
                    }
                    else if (f.missing_info_rule_ == MissingInfo::pass)
                    {
                        return true;
                    }
                }

                return data.white_elo_ >= elo;
            });
        }

        Filter max_white_elo(int elo) const
        {
            return Filter([elo](const PgnData& data, const Filter& f)
            {
                if (data.white_elo_ == any)
                {
                    if (f.missing_info_rule_ == MissingInfo::fail)
                    {
                        return false;
                    }
                    else if (f.missing_info_rule_ == MissingInfo::pass)
                    {
                        return true;
                    }
                }

                return data.white_elo_ <= elo;
            });
        }

        Filter min_black_elo(int elo) const
        {
            return Filter([elo](const PgnData& data, const Filter& f)
            {
                if (data.black_elo_ == any)
                {
                    if (f.missing_info_rule_ == MissingInfo::fail)
                    {
                        return false;
                    }
                    else if (f.missing_info_rule_ == MissingInfo::pass)
                    {
                        return true;
                    }
                }

                return data.black_elo_ >= elo;
            });
        }

        Filter max_black_elo(int elo) const
        {
            return Filter([elo](const PgnData& data, const Filter& f)
            {
                if (data.black_elo_ == any)
                {
                    if (f.missing_info_rule_ == MissingInfo::fail)
                    {
                        return false;
                    }
                    else if (f.missing_info_rule_ == MissingInfo::pass)
                    {
                        return true;
                    }
                }

                return data.black_elo_ <= elo;
            });
        }

        Filter eco(const std::string& code) const
        {
            return Filter([code](const PgnData& data, const Filter& f)
            {
                if (data.eco_.empty())
                {
                    if (f.missing_info_rule_ == MissingInfo::fail)
                    {
                        return false;
                    }
                    else if (f.missing_info_rule_ == MissingInfo::pass)
                    {
                        return true;
                    }
                }

                return data.eco_ == code;
            });
        }

        Filter opening(const std::string& line) const
        {
            return Filter([line](const PgnData& data, const Filter& f)
            {
                if (data.opening_.empty())
                {
                    if (f.missing_info_rule_ == MissingInfo::fail)
                    {
                        return false;
                    }
                    else if (f.missing_info_rule_ == MissingInfo::pass)
                    {
                        return true;
                    }
                }

                return data.opening_.starts_with(line);
            });
        }

        Filter time_control(std::string time) const
        {
            if (time.ends_with("+0"))
            {
                time.erase(time.size() - 2);
            }

            return Filter([time](const PgnData& data, const Filter& f)
            {
                if (data.time_control_.empty())
                {
                    if (f.missing_info_rule_ == MissingInfo::fail)
                    {
                        return false;
                    }
                    else if (f.missing_info_rule_ == MissingInfo::pass)
                    {
                        return true;
                    }
                }

                return data.time_control_ == time;
            });
        }

        std::string extract_header(std::ifstream& input, std::string& line)
        {
            std::string header{ line + '\n' };

            while (std::getline(input, line) && line.starts_with('['))
            {
                header += line;
                header += '\n';
            }

            return header;
        }

        void write(const std::string& input_file, const std::string& output_file, const Filter& filter)
        {
            std::ifstream input(input_file);
            if (!input)
            {
                throw std::runtime_error("Could not open the input file");
            }

            std::ofstream output(output_file);
            if (!output)
            {
                throw std::runtime_error("Could not open the output file");
            }

            bool has_line{ false };

            std::string line;
            while (has_line || std::getline(input, line)) // New game
            {
                has_line = false;

                std::string header{ extract_header(input, line) };
                parse(header);

                if (filter.evaluate(data_))
                {
                    output << header;
                    output << line << '\n';

                    // Moves
                    while (std::getline(input, line) && !line.starts_with('['))
                    {
                        output << line << '\n';
                    }
                }
                else
                {
                    if (!line.starts_with('['))
                    {
                        while (std::getline(input, line) && !line.starts_with('['))
                        {
                            continue;
                        }
                    }
                }

                if (line.starts_with('['))
                {
                    has_line = true;
                }

                header.clear();
                data_ = PgnData{};
            }
        }

        void parse(const std::string& header)
        {
            std::istringstream stream(header);
            std::string word;

            while (stream >> word)
            {
                if (word == "[Site")
                {
                    while (!word.ends_with(']') && stream >> word)
                    {
                        data_.site_ += word;
                        data_.site_ += " ";
                    }

                    if (!data_.site_.empty()) 
                    {
                        data_.site_.pop_back(); 
                    }

                    if (data_.site_.starts_with('\"'))
                    {
                        data_.site_.erase(0, 1);
                    }

                    if (data_.site_.ends_with("\"]"))
                    {
                        data_.site_.erase(data_.site_.size() - 2);
                    }
                }
                else if (word == "[White")
                {
                    while (!word.ends_with(']') && stream >> word)
                    {
                        data_.white_ += word;
                        data_.white_ += " ";
                    }

                    if (!data_.white_.empty()) 
                    {
                        data_.white_.pop_back(); 
                    }

                    if (data_.white_.starts_with('\"'))
                    {
                        data_.white_.erase(0, 1);
                    }

                    if (data_.white_.ends_with("\"]"))
                    {
                        data_.white_.erase(data_.white_.size() - 2);
                    }
                }
                else if (word == "[WhiteTitle")
                {
                    while (!word.ends_with(']') && stream >> word)
                    {
                        data_.white_title_ += word;
                        data_.white_title_ += " ";
                    }

                    if (!data_.white_title_.empty()) 
                    {
                        data_.white_title_.pop_back(); 
                    }

                    if (data_.white_title_.starts_with('\"'))
                    {
                        data_.white_title_.erase(0, 1);
                    }

                    if (data_.white_title_.ends_with("\"]"))
                    {
                        data_.white_title_.erase(data_.white_title_.size() - 2);
                    }
                }
                else if (word == "[Black")
                {
                    while (!word.ends_with(']') && stream >> word)
                    {
                        data_.black_ += word;
                        data_.black_ += " ";
                    }

                    if (!data_.black_.empty()) 
                    {
                        data_.black_.pop_back(); 
                    }

                    if (data_.black_.starts_with('\"'))
                    {
                        data_.black_.erase(0, 1);
                    }

                    if (data_.black_.ends_with("\"]"))
                    {
                        data_.black_.erase(data_.black_.size() - 2);
                    }
                }
                else if (word == "[BlackTitle")
                {
                    while (!word.ends_with(']') && stream >> word)
                    {
                        data_.black_title_ += word;
                        data_.black_title_ += " ";
                    }

                    if (!data_.black_title_.empty()) 
                    {
                        data_.black_title_.pop_back(); 
                    }

                    if (data_.black_title_.starts_with('\"'))
                    {
                        data_.black_title_.erase(0, 1);
                    }

                    if (data_.black_title_.ends_with("\"]"))
                    {
                        data_.black_title_.erase(data_.black_title_.size() - 2);
                    }
                }
                else if (word == "[Result")
                {
                    while (!word.ends_with(']') && stream >> word)
                    {
                        data_.result_ += word;
                        data_.result_ += " ";
                    }

                    if (!data_.result_.empty()) 
                    {
                        data_.result_.pop_back(); 
                    }

                    if (data_.result_.starts_with('\"'))
                    {
                        data_.result_.erase(0, 1);
                    }

                    if (data_.result_.ends_with("\"]"))
                    {
                        data_.result_.erase(data_.result_.size() - 2);
                    }
                }
                else if (word == "[Termination")
                {
                    while (!word.ends_with(']') && stream >> word)
                    {
                        data_.termination_ += word;
                        data_.termination_ += " ";
                    }

                    if (!data_.termination_.empty()) 
                    {
                        data_.termination_.pop_back(); 
                    }

                    if (data_.termination_.starts_with('\"'))
                    {
                        data_.termination_.erase(0, 1);
                    }

                    if (data_.termination_.ends_with("\"]"))
                    {
                        data_.termination_.erase(data_.termination_.size() - 2);
                    }
                }
                else if (word == "[Event")
                {
                    while (!word.ends_with(']') && stream >> word)
                    {
                        data_.event_ += word;
                        data_.event_ += " ";
                    }

                    if (!data_.event_.empty()) 
                    {
                        data_.event_.pop_back(); 
                    }

                    if (data_.event_.starts_with('\"'))
                    {
                        data_.event_.erase(0, 1);
                    }

                    if (data_.event_.ends_with("\"]"))
                    {
                        data_.event_.erase(data_.event_.size() - 2);
                    }
                }
                else if (word == "[Round")
                {
                    while (!word.ends_with(']') && stream >> word)
                    {
                        data_.round_ += word;
                        data_.round_ += " ";
                    }

                    if (!data_.round_.empty()) 
                    {
                        data_.round_.pop_back(); 
                    }

                    if (data_.round_.starts_with('\"'))
                    {
                        data_.round_.erase(0, 1);
                    }

                    if (data_.round_.ends_with("\"]"))
                    {
                        data_.round_.erase(data_.round_.size() - 2);
                    }
                }
                else if (word == "[UTCDate")
                {
                    while (!word.ends_with(']') && stream >> word)
                    {
                        data_.date_ += word;
                        data_.date_ += " ";
                    }

                    if (!data_.date_.empty()) 
                    {
                        data_.date_.pop_back(); 
                    }

                    if (data_.date_.starts_with('\"'))
                    {
                        data_.date_.erase(0, 1);
                    }

                    if (data_.date_.ends_with("\"]"))
                    {
                        data_.date_.erase(data_.date_.size() - 2);
                    }

                    try
                    {
                        data_.year_ = std::stoi(data_.date_.substr(0, 4));
                    }
                    catch (const std::invalid_argument& e)
                    {
                        data_.year_ = any;
                    }
                    
                    try
                    {
                        data_.month_ = std::stoi(data_.date_.substr(5, 2));
                    }
                    catch(const std::exception& e)
                    {
                        data_.month_ = any;
                    }

                    try
                    {
                        data_.day_ = std::stoi(data_.date_.substr(8, 2));
                    }
                    catch(const std::exception& e)
                    {
                        data_.day_ = any;
                    }
                }
                else if (word == "[WhiteElo")
                {
                    stream >> word;

                    if (word.starts_with('\"'))
                    {
                        word.erase(0, 1);
                    }

                    if (word.ends_with("\"]"))
                    {
                        word.erase(word.size() - 2);
                    }

                    try
                    {
                        data_.white_elo_ = std::stoi(word);
                    }
                    catch(const std::exception& e)
                    {
                        data_.white_elo_ = any;
                    }
                }
                else if (word == "[BlackElo")
                {
                    stream >> word;

                    if (word.starts_with('\"'))
                    {
                        word.erase(0, 1);
                    }

                    if (word.ends_with("\"]"))
                    {
                        word.erase(word.size() - 2);
                    }

                    try
                    {
                        data_.black_elo_ = std::stoi(word);
                    }
                    catch(const std::exception& e)
                    {
                        data_.black_elo_ = any;
                    }
                }
                else if (word == "[ECO")
                {
                    while (!word.ends_with(']') && stream >> word)
                    {
                        data_.eco_ += word;
                        data_.eco_ += " ";
                    }

                    if (!data_.eco_.empty()) 
                    {
                        data_.eco_.pop_back(); 
                    }

                    if (data_.eco_.starts_with('\"'))
                    {
                        data_.eco_.erase(0, 1);
                    }

                    if (data_.eco_.ends_with("\"]"))
                    {
                        data_.eco_.erase(data_.eco_.size() - 2);
                    }
                }
                else if (word == "[Opening")
                {
                    while (!word.ends_with(']') && stream >> word)
                    {
                        data_.opening_ += word;
                        data_.opening_ += " ";
                    }

                    if (!data_.opening_.empty()) 
                    {
                        data_.opening_.pop_back(); 
                    }

                    if (data_.opening_.starts_with('\"'))
                    {
                        data_.opening_.erase(0, 1);
                    }

                    if (data_.opening_.ends_with("\"]"))
                    {
                        data_.opening_.erase(data_.opening_.size() - 2);
                    }
                }
                else if (word == "[TimeControl")
                {
                    stream >> word;

                    if (word.starts_with('\"'))
                    {
                        word.erase(0, 1);
                    }

                    if (word.ends_with("\"]"))
                    {
                        word.erase(word.size() - 2);
                    }

                    if (word.ends_with("+0"))
                    {
                        word.erase(word.size() - 2);
                    }

                    data_.time_control_ = word;
                }
            }
        }
    };
}