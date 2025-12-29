#ifndef Z7_BITFIELDPROXY_H
#define Z7_BITFIELDPROXY_H

template<typename T, T BitFieldSize>
struct BitFieldProxy {
    using value_type = T;
    static constexpr T field_mask = (1 << BitFieldSize) - 1;

    value_type &value;
    const value_type field_offset; ///< Offset to the LSB of the field.

    // Assign a new value for the specified field.
    constexpr BitFieldProxy &operator=(value_type field_value) {
        value &= ~(field_mask << field_offset); // Clear existing bits
        value |= (field_value & field_mask) << field_offset; // Set new bits (being defensive with the mask)
        return *this;
    }

    // Dereference to get the current value for the field.
    constexpr value_type operator*() const { return (value >> field_offset) & field_mask; }
};

#endif // Z7_BITFIELDPROXY_H
