#include "xml-message.h"

XmlMessage::XmlMessage()
{

}

XmlMessage::~XmlMessage()
{

}

std::string XmlMessage::BuildMessage(const char* name, const char* content)
{
    TiXmlDocument doc;
    TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "", "" );
    TiXmlElement * element = new TiXmlElement( "message" );
    
    TiXmlElement * element_name = new TiXmlElement( "name" );
    TiXmlElement * element_content = new TiXmlElement( "content" );
        
    TiXmlText * text_name = new TiXmlText( name );
    TiXmlText * text_content = new TiXmlText( content );
    
    element->LinkEndChild( element_name );
    element->LinkEndChild( element_content );
    
    element_name->LinkEndChild( text_name );
    element_content->LinkEndChild( text_content );
    
    doc.LinkEndChild( decl );
    doc.LinkEndChild( element );

    stringstream ss;
    ss << doc;
 
    return ss.str(); 
 
    //elem->SetAttribute( "priority", 2 );
    //elem = elem->NextSiblingElement();
    //doc.SaveFile(filename);
}

string XmlMessage::ReadMessage(const char *str)
{
    string content;
    string name;

    if(!str)
        return "";
        
    TiXmlDocument doc;
    doc.Parse(str);
    
    TiXmlElement* root = doc.FirstChildElement();

    if(root == NULL)
    {
        cerr << "Failed to load file: No root element." << endl;
        doc.Clear();
        return "";
    }
    
    for(TiXmlElement* elem = root->FirstChildElement();
        elem != NULL;
        elem = elem->NextSiblingElement())
    {
        const char* attr;
        string elemName = elem->Value();
        //cout << "elem: " << elemName << endl;
        if(elemName == "name")
        {
            attr = elem->Attribute("level");
            if(attr != NULL);
        }
        
        else if(elemName == "content")
        {
            attr = elem->Attribute("type");
            if(attr != NULL);
        }
        
        for(TiXmlNode* e = elem->FirstChild();
            e != NULL;
            e = e->NextSibling())
        {
            TiXmlText* text = e->ToText();
            if(text == NULL)
                continue;

            if(elemName == "name")
            {
                name = text->Value();
            }
            else if (elemName == "content")
            {
                content = text->Value();
            }
        }
     }

     stringstream ss;
     ss << name << ": " << content << std::  endl;
     return ss.str();
}