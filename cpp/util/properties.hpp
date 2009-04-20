/**
 * Copyright (C) 2008 Meteor Liu
 *
 * This code has been released into the Public Domain.
 * You may do whatever you like with it.
 *
 * @file
 * @author Meteor Liu <meteor1113@gmail.com>
 * @date 2009-01-01
 */


#ifndef PROPERTIES_HPP_
#define PROPERTIES_HPP_


// #include <standard library headers>
#include <string>
#include <map>
#include <fstream>

// #include <other library headers>

// #include "customer headers"


namespace util
{

    /**
     * in Properties file,
     * #, if a line start with #, means this line is a comment;
     * any valid line must have a "=" to split key and value;
     * param can use {1}, {2}, {3} and {4}.
     */
    class Properties
    {
        typedef std::map<std::string, std::string> DataMap;

    public:
        Properties(const std::string& filename = "") { Load(filename); }
        ~Properties() {}

    private:
        //Properties(const Properties& rhs);
        //Properties& operator=(const Properties& rhs);

    public:
        void Clear() { filename_ = ""; values_.clear(); }
        void Load(const std::string& filename);
        void Save(const std::string& filename = "");
        std::string GetProperty(const std::string& key,
                                const std::string& param1 = "",
                                const std::string& param2 = "",
                                const std::string& param3 = "",
                                const std::string& param4 = "");
        void SetProperty(const std::string& key, const std::string& value);

    protected:
        std::string& Replace(std::string& s, const std::string& o,
                             const std::string& n);
        std::string& Trim(std::string& s);

    private:
        std::string filename_;
        DataMap values_;

    };

    inline void Properties::Load(const std::string& filename)
    {
        Clear();
        filename_ = filename;
        if (filename_.empty())
        {
            return;
        }

        std::ifstream fs(filename_.c_str());
        while (fs)
        {
            std::string line;
            std::getline(fs, line);
            if (line.length() > 0 && line[0] != '#')
            {
                std::string::size_type pos = line.find('=');
                if (pos != std::string::npos)
                {
                    std::string key = line.substr(0, pos);
                    std::string value =
                        line.substr(pos + 1, line.length() - pos);
                    SetProperty(Trim(key), Trim(value));
                    //SetProperty(key, value);
                }
            }
        }
        fs.close();
    }

    inline void Properties::Save(const std::string& filename = "")
    {
        std::string name = filename.empty() ? filename_ : filename;

        std::ofstream fs(name.c_str(),
                         std::ios_base::out | std::ios_base::trunc);
        for (DataMap::iterator it = values_.begin();
             it != values_.end(); ++it)
        {
            fs << it->first << "=" << it->second << std::endl;
        }
        fs.close();
    }

    inline std::string Properties::GetProperty(const std::string& key,
                                               const std::string& param1 = "",
                                               const std::string& param2 = "",
                                               const std::string& param3 = "",
                                               const std::string& param4 = "")
    {
        std::string value = key;
        DataMap::iterator it = values_.find(key);
        if (it != values_.end())
        {
            value = it->second;
        }

        Replace(value, "{1}", param1);
        Replace(value, "{2}", param2);
        Replace(value, "{3}", param3);
        Replace(value, "{4}", param4);

        return value;

    }

    inline void Properties::SetProperty(const std::string& key,
                                        const std::string& value)
    {
        values_[key] = value;
    }

    inline std::string& Properties::Replace(std::string& s,
                                            const std::string& o,
                                            const std::string& n)
    {
        std::string::size_type beg = 0;
        std::string::size_type tmp = 0;
        while (std::string::npos != (beg = (s.find(o, tmp))))
        {
            s.replace(beg, o.length(), n);
            tmp = beg + n.length();
        }
        return s;
    }

    inline std::string& Properties::Trim(std::string& s)
    {
        s.erase(s.find_last_not_of(' ') + 1);
        s.erase(0, s.find_first_not_of(' '));
        return s;
    }

    //void testProperties()
    //{
    //    util::Properties pro1("c:\\a.properties");
    //    pro1.SetProperty("dafd", "dafd");
    //    pro1.SetProperty("a", "");
    //    pro1.Save();
    //    std::string v = pro1.GetProperty("dafs");
    //    pro1.SetProperty("z", "zxcv");
    //    pro1.Save();
    //    pro1.SetProperty("v", "vv");
    //    std::string v1 = pro1.GetProperty("z");
    //}

}

#endif
