#pragma once

// Std libs
#include <map>
#include <string>
#include <memory>

// Debug
#include <mbed.h>

// Default return struct
#include "Object.hpp"

/**
 *	Macro that must be implemented in each Packages Parser class.
 *
 *	Adds a clone function to the Parser class to return a new of itself.
 *	Implemented as macro to standardise function.
 *
 *
 */
#define IMPLEMENT_SETTING_CLONE(TYPE) \
            Parser* clone() const { return new TYPE(*this); }

/**
 * Macro to be implemented for each Packages in the Settings.cpp file to recognise Parser.
 *
 * Adds a new pointer to the Parser, to the protoTable.
 *
 * @param TYPE: The Parser Class
 */
 //@todo check the "type" typo.
#define MAKE_SETTINGS_PROTOTYPE(TYPE) \
             Packages::Parser* TYPE ## __myPrototype = \
                Packages::Parser::add(#TYPE, new Packages::TYPE::TYPE());

/**
 * Cheeks to see if Package Parser has been registered.
 *
 * @param k
 * @param v
 * @param m
 * @return
 */
template <typename KEY, typename DATA>
bool find(KEY k, DATA & v, const std::map<KEY, DATA> & m)
{
	typename std::map<KEY, DATA>::const_iterator p;

	p = m.find(k);

	if (p == m.end()) {
		return false;
	}
	else {
		v = (*p).second;
		return true;
	}
}

namespace App { namespace Settings { namespace Packages {

	class Parser
	{
		public:
			/**
			 * Default Constructor/virtual Destructor.
			 */
			Parser() = default;

			virtual ~Parser() = default;

			/**
			 * Default clone to be overrode.
			 */
			virtual Parser * clone() const = 0;

			/**
			 * Makes a parser settings of parser type.
			 * @param type: std::string of the name of the parser type.
			 * @return Pointer to new parser.
			 */
			static Parser * make(std::string type)
			{
				// Get default instance
				Parser * proto = 0;

				// Find existing one
				if (!find(type, proto, protoTable)) {
					debug("DIED looking for type %s\n", type.c_str());
					exit(1); // @todo Change to mbed error.
				}

				// Return instance
				return proto->clone();
			}

			/**
			 * Adds a Parser type under a key string to the protoTable.
			 *
			 * @param type: Key to be assigned to Parser.
			 * @param p: Pointer to the parser class.
			 * @return Pointer to the parser class.
			 */
			static Parser * add(std::string type, Parser * p)
			{
				protoTable[type] = p;

				debug("Added %s\n", type.c_str());

				return p;
			}

			// Store all parsers
			static std::map<std::string, Parser *> protoTable;

			// Set m_data with Json string.
			void set(char * data)
			{
				m_data = data;
			}

			/**
			 * Get setting object
			 * @todo
			 */
			virtual std::shared_ptr<Object> get()
			{
				auto t = make_shared<Object>();
				return t;
			}

			/**
			 * Validates Json data and returns an appropriate bool based on if the data is correctly formatted for the
			 * parser.
			 *
			 * @param data: Char string of the Json data for the parser.
			 *
			 * @return: Type: Bool
			 * 			Content: True if validation of Json string passes.
			 * 					 False if validation of Json string fails.
			 */
			virtual bool validate()
			{
				return true;
			}


		protected:
			char * m_data;
	};

}}}

