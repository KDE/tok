// SPDX-FileCopyrightText: 2021 Carson Black <uhhadd@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

// helper function yoinked from tdlib example
namespace detail
{
template<class... Fs>
struct overload;

template<class F>
struct overload<F> : public F {
    explicit overload(F f)
        : F(f)
    {
    }
};
template<class F, class... Fs>
struct overload<F, Fs...> : public overload<F>, overload<Fs...> {
    overload(F f, Fs... fs)
        : overload<F>(f)
        , overload<Fs...>(fs...)
    {
    }
    using overload<F>::operator();
    using overload<Fs...>::operator();
};
}

template<class... F>
auto overloaded(F... f)
{
    return detail::overload<F...>(f...);
}
