#pragma once

#ifndef MYGETTERSETTER
/**
 * A utility to build get and set methods on top of a property.
 *
 * @param type The type of the property.
 * @param method The human-readable name for the method.
 * @param name The machine-readable name of the property.
 */
#define MYGETTERSETTER(type, method, name)                                     \
    /** Retrieves the name value for the object. @return The name value of the \
     * object. */                                                              \
    type get_##method() const { return name; }                                 \
    /** Sets the name value for the object. @param value The value of which to \
     * set name to. */                                                         \
    void set_##method(type value) { name = value; }

#define MYGETTER(type, method, name)                                           \
    /** Retrieves the name value for the object. @return The name value of the \
     * object. */                                                              \
    type get_##method() const { return name; }

#define MYSETTER(type, method, name)                                           \
    /** Sets the name value for the object. @param value The value of which to \
     * set name to. */                                                         \
    void set_##method(type value) { name = value; }

#endif
