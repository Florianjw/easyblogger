#include "settings.hpp"
#include "files.hpp"
#include "utility"
#include "lines.hpp"
#include "blogentry.hpp"

#include <cstdlib>
#include <iostream>
#include <stdexcept>

using std::pair;

settings read_settings(string filename){
	settings S;
	string val;
	pair<string,string> line;
	LINES lines=read_config_file(filename);
	
	S.conf_file=filename;
	///SOME DEFAULTS:
	S.max_date_size=-1;
	S.number_of_mainpageposts=-1;
	S.toc_in_singleentries=false;
	S.sorting_by_date=true;
	S.enable_comments=true;
	S.enable_tag_file=false;
	S.tag_feeds=false;
	S.tag_feeds_description="%s";
	S.rss_file_extension=".rss";
	S.filename_extension=".html";
	
	for(LINES::iterator it=lines.begin();it!=lines.end();++it){
		line=cut(*it);
		val=line.first;
		if(val=="settingsdir"){
			S.settingsdir=line.second;
		}
		else if(val=="name"){
			S.name=line.second;
		}
		else if(val=="datadir"){
			S.datadir=line.second;
		}
		else if(val=="blog"){
			S.blog=line.second;
		}
		else if(val=="list_of_entries"){
			S.list_of_entries=line.second;
		}
		else if(val=="single_entries_dir"){
			S.single_entries_dir=line.second;
		}
		else if(val=="single_entries_dir_rel"){
			S.single_entries_dir_rel=line.second;
		}
		else if(val=="url"){
			S.url=line.second;
		}
		else if(val=="editor"){
			S.editor=line.second;
		}
		else if(val=="filename_extension"){
			S.filename_extension=line.second;
		}
		else if(val=="comment_heading"){
			S.comment_section_heading=line.second;
		}
		else if(val=="comment_url"){
			S.comment_url=line.second;
		}
		else if(val=="comment_name"){
			S.comment_name=line.second;
		}
		else if(val=="locale"){
			S.locale=line.second;
		}
		else if(val=="time_format"){
			S.time_format=line.second;
		}
		else if(val=="RSS_file"){
			S.rss_feed=line.second;
		}
		else if(val=="RSS_channel_description_file"){
			S.rss_channel_description_file=line.second;
		}
		else if(val=="toc_title"){
			S.toc_title=line.second;
		}
		else if(val=="toc_pre"){
			S.toc_pre=line.second;
		}
		else if(val=="toc_post"){
			S.toc_post=line.second;
		}
		else if(val=="mainpage_toc_title"){
			S.mainpage_toc_title=line.second;
		}
		else if(val=="mainpage_toc_pre"){
			S.mainpage_toc_pre=line.second;
		}
		else if(val=="mainpage_toc_post"){
			S.mainpage_toc_post=line.second;
		}
		else if(val=="number_of_mainpageposts"){
			S.number_of_mainpageposts=atoi(line.second.c_str());
		}
		else if(val=="tags_title"){
			S.tags_title=line.second;
		}
		else if(val=="tag_file"){
			S.tag_file=line.second;
		}
		else if(val=="tag_file_rel"){
			S.tag_file_rel=line.second;
		}
		else if(val=="tag_file_template"){
			S.tag_file_template=line.second;
		}
		else if(val=="max_date_size"){
			S.max_date_size=atoi(line.second.c_str());
		}
		else if(val=="id_file"){
			ID id;
			id.read_file(line.second);
			S.last_id=id;
		}
		else if(val=="template_file"){
			S.template_file=line.second;
		}
		else if(val=="toc_in_singleentries"){
			if(line.second=="true"){
				S.toc_in_singleentries=true;
			}
			else if(line.second=="false"){
				S.toc_in_singleentries=false;
			}
		}
		else if(val=="sorting_by_date"){
			if(line.second=="true"){
				S.sorting_by_date=true;
			}
			else if(line.second=="false"){
				S.sorting_by_date=false;
			}
			
		}
		else if(val=="enable_comments"){
			if(line.second=="true"){
				S.enable_comments=true;
			}
			else if(line.second=="false"){
				S.enable_comments=false;
			}
		}
		else if(val=="enable_tag_file"){
			if(line.second=="true"){
				S.enable_tag_file=true;
			}
			else if(line.second=="false"){
				S.enable_tag_file=false;
			}
		}
		else if(val=="tag_feeds"){
			if(line.second=="true"){
				S.tag_feeds=true;
			}
			else if(line.second=="false"){
				S.tag_feeds=false;
			}
		}
		else if(val=="tag_feeds_dir"){
			S.tag_feeds_dir=line.second;
		}
		else if(val=="tag_feeds_dir_rel"){
			S.tag_feeds_dir_rel=line.second;
		}
		else if(val=="tag_feeds_description"){
			S.tag_feeds_description=line.second;
		}
		else{
			cerr<<"Unknwon key: "<<val<<" ; (value: "<<line.second<<")"<<endl;
		}
	}
	
	///Checks:	
	if(S.tag_feeds){
		if(S.tag_feeds_dir.empty()){
			S.tag_feeds=false;
		}
	}
	
//	LINES entries_file=read_file(S.list_of_entries);
//	for(LINES::iterator it=entries_file.begin();it!=entries_file.end();++it){
//		S.blogentries.push_back(new blogentry(*it,S));
//	}
	
	return S;
}

settings::~settings(){
	for(list<blogentry*>::iterator it=blogentries.begin();it!=blogentries.end();++it){
		delete *it;
	}
}


struct _blog_{
	deque<string> names;
	string conf_file;
	bool default_entry;
};

struct list_of_blogs{
	deque<_blog_> blogs;
	deque<_blog_>::iterator default_blog_it;
};

list_of_blogs get_list_of_blogs(){
	_blog_ *tempval=NULL;
	list_of_blogs blogs_list;
	blogs_list.default_blog_it=blogs_list.blogs.end();
	LINES data=read_config_file(GLOBAL_EASYBLOGGER_CONFIG_FILE);
	bool entry_open=false;
	string line;
	for(LINES::iterator it=data.begin();it!=data.end();++it){
		line=*it;
		line=clean_whitespace(line);
		if(entry_open){
			if(line.find(END_BLOG_CONFIG)==0){
				entry_open=false;
				blogs_list.blogs.push_back(*tempval);
				if(tempval->default_entry){
					blogs_list.default_blog_it=--(blogs_list.blogs.end());
				}
				delete tempval;
				tempval=NULL;
				continue;
			}
			else if(line.find("config=")==0){
				tempval->conf_file=cut(line,"=").second;
				continue;
			}
			else if(line.find("names=")==0){
				deque<string> names=cut_words(cut(line,"=").second);
				for(deque<string>::iterator it=names.begin();it!=names.end();++it){
					tempval->names.push_back(*it);
				}
			}
			else if(line.find("name")==0){
				tempval->names.push_back(cut(line,"=").second);
			}
			else if(line.find(DEFAULT_INDICATOR)==0){
				tempval->default_entry=true;
			}
			else{
				std::cerr<<"WARNING: INVALID DATA IN THE CONFIGURATION-FILE: "<<line<<std::endl;
				continue;
			}
		}
		else{
			if(line.find(BEGIN_BLOG_CONFIG)==0){
				entry_open=true;
				tempval=new _blog_;
				tempval->names=cut_words(line.substr(BEGIN_BLOG_CONFIG.size()+1));
				tempval->default_entry=false;
				for(deque<string>::iterator it=tempval->names.begin();it!=tempval->names.end();++it){
					if(*it==DEFAULT_INDICATOR){
						tempval->default_entry=true;
						break;
					}
				}
			}
		}
	}
	if(tempval!=NULL){
		delete tempval;
		tempval=NULL;
	}
	return blogs_list;
}


settings get_blog_by_name(string name){
	list_of_blogs blogs=get_list_of_blogs();
	settings S;
	if(name==DEFAULT_INDICATOR){
		if(blogs.default_blog_it==blogs.blogs.end()){
			throw std::logic_error("does not exist");
		}
		return read_settings(blogs.default_blog_it->conf_file);
	}
	else{
		_blog_ tempval;
		for(deque<_blog_>::iterator it=blogs.blogs.begin();it!=blogs.blogs.end();++it){
			tempval=*it;
			for(deque<string>::iterator it=tempval.names.begin();it!=tempval.names.end();++it){
				if(*it==name){
					return read_settings(get_blog_conf_file(*it));
				}
			}
		}
	}
	throw std::logic_error("does not exist");
}

string get_blog_conf_file(string name){
	list_of_blogs blogs=get_list_of_blogs();
	if(name==DEFAULT_INDICATOR){
		return blogs.default_blog_it->conf_file;
	}
	else{
		_blog_ tempval;
		for(deque<_blog_>::iterator it=blogs.blogs.begin();it!=blogs.blogs.end();++it){
			tempval=*it;
			for(deque<string>::iterator it_name=tempval.names.begin();it_name!=tempval.names.end();++it_name){
				if(*it_name==name){
					return it->conf_file;
				}
			}
		}
	}
	throw std::logic_error("does not exist");
}
