#ifndef Z7_BITFIELDPROXY_H
#define Z7_BITFIELDPROXY_H

template<typename T, T FIELD_MASK>
struct BitFieldProxy {
    using value_type = T;

    value_type &value;
    const value_type field_offset; ///< Offset to the LSB of the field.

    // Assign a new value for the specified field.
    constexpr BitFieldProxy &operator=(value_type field_value) {
        value &= ~(FIELD_MASK << field_offset); // Clear existing bits
        value |= (field_value & FIELD_MASK) << field_offset; // Set new bits (being defensive with the mask)
        return *this;
    }

    // Dereference to get the current value for the field.
    constexpr value_type operator*() const { return (value >> field_offset) & FIELD_MASK; }
};

#endif // Z7_BITFIELDPROXY_H
