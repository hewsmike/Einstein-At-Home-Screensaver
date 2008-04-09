#ifndef RESOURCEFACTORY_H_
#define RESOURCEFACTORY_H_

#include <string>
#include <map>
#include <iostream>

#include "Resource.h"

/**
 * \brief This factory instantiates Resource objects using a given identifier
 * 
 * During contruction of the factory object itself it loads all available resources into
 * an internal cache. The user can subsequently request a copy of this resource by specifying
 * it using the resource's identifier.
 *
 * The resource data is expected to be stored in these three externally linked arrays:
 * <ul>
 * 	<li>\ref c_ResourceIdentifiers</li>
 * 	<li>\ref c_ResourceIndex</li> 
 * 	<li>\ref c_ResourceStorage</li>
 * </ul>
 * These arrays are compiled using the Open Resource Compiler (ORC) which can be found
 * in the \c orc subdirectory.
 * 
 * \see Resource
 * \see ResourceCompiler
 *  
 * \author Oliver Bock\n
 * Max-Planck-Institute for Gravitational Physics\n
 * Hannover, Germany
 */
class ResourceFactory
{
public:
	/// Default constructor
	ResourceFactory();
	
	/// Destructor
	virtual ~ResourceFactory();
	
	/**
	 * \brief Creates an instance of the requested resource
	 * 
	 * The identifier has to be the same as the one specified in the \c *.orc file
	 * used to compile the resources
	 * 
	 * \param identifier The identifer of the resource to be instantiated
	 * 
	 * \return A constant pointer to the newly instantiated resource
	 * or NULL if the specified resource could not be found
	 * 
	 * \see ResourceCompiler
	 */
	const Resource* createInstance(const string identifier);
	
private:
	/// Hashtable mapping identifiers to resource data
	map<string, vector<unsigned char> > m_ResourceMap;
};

/**
 * \brief Declaration of the external list of resource identifiers
 * 
 * \todo Does this need to be global?
 * Maybe we should wrap a class around the generated resources?
 */
extern const string c_ResourceIdentifiers[];

/**
 * \brief Declaration of the external list of resource indices
 * 
 * This two-dimensional array contains the necessary indices for
 * every resource in the storage container. Using these indices you
 * can easily extract a requested resource:
 * <ol>
 * 	<li>Offset to the resource</li>
 * 	<li>Length of the resource</li>
 * </ol> 
 * 
 * \see c_ResourceStorage
 * 
 * \todo Does this need to be global?
 * Maybe we should wrap a class around the generated resources?
 */
extern const unsigned int c_ResourceIndex[][2];

/**
 * \brief Declaration of the external resource storage container
 * 
 * The container comprises all resources in a continuous byte sequence
 * expressed as hex values. Use the resource index to locate/retrieve
 * a specific resource from the container.
 * 
 * \see c_ResourceIndex
 * 
 * \todo Does this need to be global?
 * Maybe we should wrap a class around the generated resources?
 */
extern const unsigned char c_ResourceStorage[];

#endif /*RESOURCEFACTORY_H_*/
