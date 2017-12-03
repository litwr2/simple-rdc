# simple-rdc
It is a simple recurse descent parser for arithmetic formulas.  It converts a formula in infix notation to Reverse Polish notation (RPN).  It also evaluates the integer formula value.  It can easily be modified for usage of the arithmetic of real numbers.

It works with any formula compliant with the next grammar in BNF:

---
>Formula **->** Formula + Term **|** Formula - Term
>Term **->** Term * Component **|** Term / Component
>Component **->** + Component **|** - Component **|** Item
>Item **->** Element **|** Element ^ Component
>Element **->** (Formula) **|** number

---

The grammar above is left recursive so the parser has to use the equivalent grammar in EBNF:

---
>Formula **->** Term **{** AddSub Term **}**
>AddSub **->** + **|** -
>Term **->** Component **{** MulDiv Component **}**
>MulDiv **->** * **|** /
>Component **->** + Component **|** - Component **|** Item
>Item **->** Element **|** Element ^ Component
>Element **->** (Formula) **|** number

---


