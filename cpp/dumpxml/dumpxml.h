/* -*- mode: C++; -*- */
#pragma once
//************************************
// Author:              oneaddone
//************************************

#ifndef DUMPXML_H
#define DUMPXML_H

#include <string>
#include <deque>
#include <vector>
#include <list>
#include <map>
#include "tinyxml/tinyxml.h"
#include "boost/lexical_cast.hpp"

namespace dumpxml
{
    using namespace std;

    template<typename Target, typename Source>
    Target
    my_cast(const Source* v)
    {
        if (v != NULL)
        {
            try
            {
                return boost::lexical_cast<Target>(v);
            }
            catch (boost::bad_lexical_cast &)
            {
                return Target();
            }
        }
        return Target();
    }

    template<typename Target, typename Source>
    Target
    my_cast(const Source& v)
    {
        try
        {
            return boost::lexical_cast<Target>(v);
        }
        catch (boost::bad_lexical_cast &)
        {
            return Target();
        }
    }

    template<class T, class U>
    class Conversion
    {
        typedef char Small;
        class Big
        {
            char dummy[2];
        };
        static Small Test(U);
        static Big Test(...);
        static T MakeT();
    public:
        enum { exists = sizeof(Test(MakeT())) == sizeof(Small) };
    };

    //////////////////////////////////////////////////////////////////////////

    struct TiXmlBindCustom {};

    template<typename T, bool IsCustom = Conversion<T, TiXmlBindCustom>::exists>
    struct TiXmlBind : public T
    {
        typedef T value_type;
    };

    template<typename T>
    struct TiXmlBind<T, false>
    {
        typedef T value_type;

        static TiXmlElement *GetTiXml(const value_type &val,
                                      const string & Name = "item")
            {
                TiXmlElement* Node = new TiXmlElement(Name.c_str());
                Node->LinkEndChild(
                    new TiXmlText(my_cast<string, value_type>(val).c_str()));
                return Node;
            }

        static value_type TiXmlGetValue(const TiXmlNode * Node)
            {
                if (Node != NULL)
                {
                    Node = Node->FirstChild();
                }
                if (Node != NULL)
                {
                    return my_cast<value_type, const char*>(Node->Value());
                }
                else
                {
                    return value_type();
                }
            }
    };

    template<typename T,
             template<class Type, class Allocator> class Container = vector>
    struct TiXmlBindSequence
    {
        typedef typename T::value_type element_type;
        typedef Container<element_type, allocator<element_type> > value_type;

        static TiXmlElement* GetTiXml(const value_type& val,
                                      const string& Name = "sequence")
            {
                TiXmlElement* Node = new TiXmlElement(Name.c_str());
                for (typename value_type::const_iterator it = val.begin();
                     it != val.end(); it++)
                {
                    Node->LinkEndChild(T::GetTiXml(*it));
                }
                return Node;
            }

        static value_type TiXmlGetValue(const TiXmlNode* Node)
            {
                value_type s;
                if (Node != NULL)
                {
                    Node = Node->FirstChild();
                }
                for ( ; Node; Node = Node->NextSibling())
                {
                    s.push_back(T::TiXmlGetValue(Node));
                }
                return s;
            }
    };

    template<typename K, typename V>
    struct TiXmlBindMap
    {
        typedef typename K::value_type key_element_type;
        typedef typename V::value_type element_type;
        typedef map<key_element_type, element_type> value_type;

        static TiXmlElement *GetTiXml(const value_type &val,
                                      const string & Name = "map")
            {
                TiXmlElement * Node = new TiXmlElement(Name.c_str());
                for (typename value_type::const_iterator it = val.begin();
                     it != val.end(); it++)
                {
                    TiXmlElement * Child = V::GetTiXml(it->second);
                    Child->SetAttribute("key", K::GetTiXml(it->first)->FirstChild()->ToText()->Value());
                    Node->LinkEndChild(Child);
                }
                return Node;
            }

        static value_type TiXmlGetValue(const TiXmlNode * Node)
            {
                value_type s;
                if (Node != NULL)
                {
                    Node = Node->FirstChild();
                }
                for ( ; Node; Node = Node->NextSibling())
                {
                    value_type s;
                    if (Node) Node = Node->FirstChild();
                    for ( ; Node; Node = Node->NextSibling())
                    {
                        const char * keystr = Node->ToElement()->Attribute("key");
                        if (NULL == keystr) keystr = Node->Value();
                        key_element_type key = my_cast<key_element_type, const char*>(keystr);
                        s.insert(typename value_type::value_type(key, V::TiXmlGetValue(Node)));
                    }
                    return s;
                }
                return s;
            }
    };

    //////////////////////////////////////////////////////////////////////////

    template<class T>
    struct BindTraits
    {
        typedef TiXmlBind<T> bind_type;
    };

    template<class T>
    struct BindTraits<vector<T> >
    {
        typedef TiXmlBindSequence<typename BindTraits<T>::bind_type, vector> bind_type;
    };

    template<class T>
    struct BindTraits<list<T> >
    {
        typedef TiXmlBindSequence<typename BindTraits<T>::bind_type, list> bind_type;
    };

    template<class T>
    struct BindTraits<deque<T> >
    {
        typedef TiXmlBindSequence<typename BindTraits<T>::bind_type, deque> bind_type;
    };

    template<class K, class V>
    struct BindTraits<map<K, V> >
    {
        typedef TiXmlBindMap<typename BindTraits<K>::bind_type, typename BindTraits<V>::bind_type > bind_type;
    };

    //////////////////////////////////////////////////////////////////////////

    template<class T>
    TiXmlNode*
    VarToTiXml(const T &val, const string & Name)
    {
        return BindTraits<T>::bind_type::GetTiXml(val, Name);
    }

    template<class T>
    TiXmlNode*
    VarToTiXmlChild(const T &val, TiXmlNode * Node, const string & Name)
    {
        if (Node != NULL)
        {
            return Node->LinkEndChild(VarToTiXml(val, Name));
        }
        else
        {
            return NULL;
        }
    }

    template<class T>
    bool
    VarToTiXmlFile(const T &val, const string & FileName,
                   const string &Encoding = "UTF-8")
    {
        TiXmlDocument doc;
        doc.LinkEndChild(new TiXmlDeclaration("1.0", Encoding.c_str(), ""));
        doc.LinkEndChild(VarToTiXml(val, "root"));

        if (doc.SaveFile(FileName.c_str()))
        {
            return true;
        }
        return false;
    }

    template<class T>
    void
    TiXmlToVar(T &val, const TiXmlNode * Node)
    {
        val = BindTraits<T>::bind_type::TiXmlGetValue(Node);
    }

    template<class T>
    void
    TiXmlChildToVar(T &val, const TiXmlNode * Node, const string & Name)
    {
        if (Node != NULL)
        {
            TiXmlToVar(val, Node->FirstChild(Name.c_str()));
        }
        else
        {
            val = T();
        }
    }

    template<class T>
    bool
    TiXmlFileToVar(T &val, const string & FileName)
    {
        TiXmlDocument doc;
        TiXmlNode * Node;
        if (doc.LoadFile(FileName.c_str())
            && (Node = doc.FirstChildElement()))
        {
            TiXmlToVar(val, Node);
            return true;
        }
        return false;
    }

    //////////////////////////////////////////////////////////////////////////

    struct TiXmlTieBase
    {
        string m_NodeName;
        TiXmlTieBase(const string &Name)
            :m_NodeName(Name)
            {}

        virtual ~TiXmlTieBase() {}

        virtual void SetValue(TiXmlNode * Parent) = 0;
        virtual TiXmlNode * LinkChild(TiXmlNode * Parent) = 0;
    };

    template<class T>
    struct TiXmlTie : public TiXmlTieBase
    {
        typedef typename T::value_type element_type;
        element_type &m_RefValue;

        TiXmlTie(element_type &Ref, const string &Name)
            :m_RefValue(Ref)
            ,TiXmlTieBase(Name)
            {}

        virtual void SetValue(TiXmlNode * Parent)
            {
                TiXmlElement * Node;
                if (Parent
                    && (Node = Parent->FirstChildElement(m_NodeName.c_str())))
                {
                    m_RefValue = T::TiXmlGetValue(Node);
                }
                else
                {
                    m_RefValue = element_type();
                }
            }

        virtual TiXmlNode * LinkChild(TiXmlNode * Parent)
            {
                return Parent->LinkEndChild(
                    T::GetTiXml(m_RefValue, m_NodeName.c_str()));
            }
    };

    struct TiXmlTieSaveLoad
    {
        list<TiXmlTieBase *> m_TiXmlVars;

        typedef list<TiXmlTieBase *>::iterator tie_iterator;

        TiXmlTieSaveLoad()
            {
            }

        ~TiXmlTieSaveLoad()
            {
                for (tie_iterator it = m_TiXmlVars.begin();
                     it != m_TiXmlVars.end(); ++it)
                {
                    delete *it;
                }
            }

        void AddTiXmlTie(TiXmlTieBase * Var)
            {
                m_TiXmlVars.push_back(Var);
            }

        template<class T>
        void AddTiXmlTie(T & var, const string & Name)
            {
                AddTiXmlTie(
                    new TiXmlTie<typename BindTraits<T>::bind_type>(var, Name));
            }

        bool LoadFile(const string &FileName)
            {
                TiXmlDocument doc;
                if (doc.LoadFile(FileName.c_str()))
                {
                    TiXmlElement * Node = doc.FirstChildElement();
                    if (Node != NULL)
                    {
                        for (tie_iterator it = m_TiXmlVars.begin();
                             it != m_TiXmlVars.end(); ++it)
                        {
                            (*it)->SetValue(Node);
                        }
                        return true;
                    }
                }
                return false;
            }

        bool SaveFile(const string &FileName, const string &Encoding = "UTF-8")
            {
                TiXmlDocument doc;
                doc.LinkEndChild(
                    new TiXmlDeclaration( "1.0", Encoding.c_str(), "" ) );
                TiXmlElement * Node = new TiXmlElement("root");
                doc.LinkEndChild(Node);

                for (tie_iterator it = m_TiXmlVars.begin();
                     it != m_TiXmlVars.end(); ++it)
                {
                    (*it)->LinkChild(Node);
                }

                if (doc.SaveFile(FileName.c_str()))
                {
                    return true;
                }
                return false;
            }
    };

}

#endif
