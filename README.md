# Data Structure - Homework 6: Calculator

## 目的(Goal)

> 以 C / C++ 語言實做一簡單計算機程式, 輸入 infix 表示式的數學運算式,
> 輸出運算式的計算結果. 不保證輸入運算式合法, 需提供偵錯功能.

### 錯誤型態(Error Type)
- Value Error (VE)
	+ Illegal character (UN)
	+ ~~Illegal operand (UN)~~
- Runtime Error (RE)
	+ Divide By Zero (DZ)
	+ Integer Underflow (UF)
	+ Integer Overflow (OF)
- Syntax Error (SE)
	+ Unmatched Parenthesis (UMPA)
		- Unmatched left parenthesis (UMPA_L)
		- Unmatched right parenthesis (UMPA_R)
	+ Empty Expression (EMPTY)
		* Left parenthesis followed by a right parenthesis (EMPTY)
	+ First character an operator (FOPT)
		* Left parenthesis followed by an operator (FOPT_L)
			- exception: negative value
	+ Last character an operator (LOPT)
		* Operator followed by a right parenthesis (LOPT_R)
	+ Operator followed by an operator (OPTOPT)
- Nonsupport Feature (MISMUL)
	+ Identifier followed by a left parenthesis (MISMUL_L)
	+ Right parenthesis followed by an identifier (MISMUL_R)
	+ Right parenthesis followed by a left parenthesis (MISMUL)

### 範例(Example)
- Non-Error
```bash
cal "3+2-1"
>>> 4
cal "3-2+1"
>>> 2
cal "1+2*3"
>>> 7
cal "1*2+3	"
>>> 5
cal "(1+2)*3"
>>> 9
cal "1*(2+3)"
>>> 5
```
- VE
```bash
cal "A!"
>>> UN
cal "0/0"
>>>
```
- SE
```bash
cal "()"
>>> "EMPTY"
cal ""
>>>
```

---

### 符號定義(Symbol Definition)
- 運算式(expression): `<exp> := <sym> | <sym><exp>`
- 符號(Symbol): `<sym> := <opt> | <val>`
- 運算子(operator): `<opt> := [+-*/]`
	+ 優先度(priority): `{'+': 0, '-': 1, '*': 2, '/': 3}}`
- 運算元(operand):
	+ 純量(scale): `<val> := [-]?[0-9]+`
		* 十進制(decimal)

---

## System
### System Design
- `<simple-exp> = <node>[]`
- `<node> := <val>|<opt>`

- Define `<simple-exp> := <val>|<simple-exp><opt><simple-exp>`
- `declare function cal(<simple-exp>) return <val>`



## License
Copyright (C) 2020 Hung-Hsiang, Lin

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sub-license, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.



