#ifndef TEMPLATES_TARITHMETICABLE_H_
#define TEMPLATES_TARITHMETICABLE_H_

namespace templates
{

template<typename Derived, typename OutType>
struct TArithmeticable
{
  virtual ~TArithmeticable() = default;
  virtual void reset(const OutType& in) = 0;
  virtual OutType operator () () const = 0;
  virtual bool operator < (const Derived& o2) const = 0;

  Derived& operator = (const Derived& o2) {
    Derived *p1 = static_cast<Derived*>(this);
    if (p1 == &o2)
      return *p1;
    reset(o2());
    return *p1;
  }

  Derived& operator = (const OutType& o2) {
    reset(o2);
    return *static_cast<Derived*>(this);
  }

  Derived& operator += (const Derived& o2) {
    reset((*this)() + o2());
    return *static_cast<Derived*>(this);
  }

  Derived& operator -= (const Derived& o2) {
    reset((*this)() - o2());
    return *static_cast<Derived*>(this);
  }
};

template<typename Derived, typename OutType>
bool operator == (const TArithmeticable<Derived, OutType>& o1,
                  const TArithmeticable<Derived, OutType>& o2)
{
  const Derived& d1 = static_cast<const Derived&>(o1);
  const Derived& d2 = static_cast<const Derived&>(o2);
  return (!(d1 < d2) && !(d2 < d1));
}

template<typename Derived, typename OutType>
bool operator > (const TArithmeticable<Derived, OutType>& o1,
                 const TArithmeticable<Derived, OutType>& o2)
{
  const Derived& d1 = static_cast<const Derived&>(o1);
  const Derived& d2 = static_cast<const Derived&>(o2);
  return (d2 < d1);
}

template<typename Derived, typename OutType>
bool operator != (const TArithmeticable<Derived, OutType>& o1,
                  const TArithmeticable<Derived, OutType>& o2)
{
  return !(o1 == o2);
}

template<typename Derived, typename OutType>
bool operator <= (const TArithmeticable<Derived, OutType>& o1,
                  const TArithmeticable<Derived, OutType>& o2)
{
  return ((o2 > o1) || (o1 == o2));
}

template<typename Derived, typename OutType>
bool operator >= (const TArithmeticable<Derived, OutType>& o1,
                  const TArithmeticable<Derived, OutType>& o2)
{
  return ((o1 > o2) || (o1 == o2));
}

template<typename Derived, typename OutType>
Derived operator + (const TArithmeticable<Derived, OutType>& o1,
                    const TArithmeticable<Derived, OutType>& o2)
{
  const Derived& d1 = static_cast<const Derived&>(o1);
  const Derived& d2 = static_cast<const Derived&>(o2);
  return Derived(d1() + d2());
}

template<typename Derived, typename OutType>
Derived operator - (const TArithmeticable<Derived, OutType>& o1,
                    const TArithmeticable<Derived, OutType>& o2)
{
  const Derived& d1 = static_cast<const Derived&>(o1);
  const Derived& d2 = static_cast<const Derived&>(o2);
  return Derived(d1() - d2());
}

} // namespace templates

#endif // TEMPLATES_TARITHMETICABLE_H_
