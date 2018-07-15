#include "string.h"
#include "datetime.h"
#include "queries.h"
#include "items.h"
#include "lines.h"
#include "components.h"
#include "elements.h"
#include <unordered_map>

#if !defined(__MESSAGE)
#define __MESSAGE

using namespace global;

namespace data
{
	namespace message
	{
		/*
		class base : public json
		{
		public:
			int messageID;
			int userID;
			datetime created;
			datetime finished;

		public:
			base() { clear(); }
			base(json *parent) { clear(); json::parent(parent); }
			base(base const &source) { clear(); copy(source); }

			void clear();
			void copy(base const &source);

		public:
			string identifier() { return string("MESSAGE"); }
			bool add(custom::pair &source);

		public:
			base& operator=(const base& source)
			{
				this->copy((base&)source);
				return *this;
			}
		};
		*/
		class message : public json//base
		{
			struct equality
			{
				bool operator()(const string &str1, const string &str2) const
				{
					return ((string&)str1).icompare((string&)str2);
				}
			};

			struct hasher
			{
				std::size_t operator()(const string& k) const
				{
					return (std::size_t)(((string&)k).upper()).hash();
				}
			};

			std::unordered_map<string, data::json *, hasher, equality> hash;
			std::unordered_map<string, queue::base *, hasher, equality> queue_hash;

		public:
			int messageID;
			int userID;
			datetime created;
			datetime finished;

		private:
			bool init;

		//public:
			//data::items::items<MAX> *items;
		public:
			//data::item::base item;

			data::queries::queries<10L> queries;
			data::items::items<10L> items;
			data::lines::lines<10L> lines;
			data::components::components<10L> components;
			data::elements::elements<10L> elements;
			//allocator::allocator<10L> lines;
			//allocator::allocator<10L> components;
			//allocator::allocator<data::element::element, 10L> elements;

		public:
			message() { makeNull(); reset(); }
			message(message const &source) { makeNull(); reset(); copy(source); }
			~message() { cleanup(); }

			bool initalised() { return init; }
			void reset();

			void clear();// override;

			queue::base *findQ(string FQDN);
			data::json *find(string FQDN);
			//data::json *find(string label);

			void copy(message const &source);

		public:
			string identifier() { return string("MESSAGE"); }
			bool add(custom::pair &source);

		public:
			message& operator=(const message& source)
			{
				this->copy((message&)source);
				return *this;
			}

		protected:
			void makeNull();
			void cleanup();
		};
	};
};

#endif