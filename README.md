# Data Structure - Homework 6: Calculator

## 目的(Goal)

> 以 C / C++ 語言實做一簡單計算機程式, 輸入 infix 表示式的數學運算式,
> 輸出運算式 postfix 表達式與其計算結果. 不保證輸入運算式合法, 需提供偵錯功能.

### 錯誤型態(Error Type)
- Line contains more characters than allowed.
- Value Error
	+ Illegal character
- Syntax Error
	+ Unmatched Parenthesis
		- Unmatched left parenthesis
		- Unmatched right parenthesis
	+ Empty Expression
		* Left parenthesis followed by a right parenthesis (EMPTY)
	+ First character an operator
		* Left parenthesis followed by an operator
			- except: negative value
	+ Last character an operator
		* Operator followed by a right parenthesis
	+ Operator followed by an operator
- Nonsupport Feature
	+ Identifier followed by a left parenthesis
	+ Right parenthesis followed by an identifier
	+ Right parenthesis followed by a left parenthesis
- Runtime Error
	+ Divide By Zero

### 範例(Example)
參考 [sample_input](sample_input), [sample_output](sample_output)

---

## 如何使用... (How to ...)
### 建置
```shell
make all
```

### 測試
```shell
make test
```

### 刪除
```shell
make clean
```

## 輸入/輸出介面(Interface)
載入後無提示, 輸入運算式後會於下行輸出計算結果,
如輸入有誤, 則輸出錯誤資訊(可能不只一條),
每筆運算結束後會詢問是否繼續,
敲擊 `ESC`, `n`, `N` 鍵離開程序

```shell
$ ./main
1+1
The postfix expression: 11+= 2
Continue? [Y/n]
```

---

## 分析(Analyze)
### 定義(Definition)
- 運算式(expression): `<exp> := <sym> | <sym><exp>`
- 符號(Symbol): `<sym> := <opt> | <val>`
- 運算子(operator): `<opt> := [+-*/()]`
	+ 優先度(priority):
		* `+`, `-`: 1 (lowest)
		* `*`, `/`: 2
		* `(`, `)`: 3 (highest)
- 運算元(operand):
	+ 純量(scale): `<val> := [-]?[.0-9]+`
		* 十進制(decimal)

### 處理步驟(Process)
> 首先, 考慮如何將輸入的字串拆分成一個個僅由運算子或運算元構成的小段
> 參考運算式的定義, 可知命題同等於:
> "找到一個方法, 將運算式拆分為一個符號跟一個後綴的運算式"
> 以此為基礎, 可以構造第一個函數使得:
> `"1+(-1)"` 變為 `["1", "+", "(", "-", 1", ")"]`

```c
// functional.h
void generate_token (
	node** destination,
	const char* source
);
```

> 接著, 考慮負數的表達式, 我們要將產生的 token-list 做一些預處理,
> 讓 `["(", "-", 1", ")"]` 變為 `["-1"]`,
> 並將該預處理加入前一函數尾部.

> 接下來, 由於**不保證輸入運算式合法**, 構建一個語法檢查的函數,
> 以偵測給定錯誤型態列表中的錯誤, 並發出相關資訊.
> 同時為方便查找是否有錯誤發生, 配置的一個錯誤計數器與其相關函數.

```c
// functional.h
int error_counter;
void syntax_check (
	node* source
);

// functional.c
static void raise_error(
	const char* error_message
);  // error_counter++
```

> 如果語法檢查通過, 接下來考慮將生成的 token-list 轉為樹狀結構 (tree):

> 考慮一最簡單的運算式 `["1"]`,
> 很顯然的將其放置於根節點即可,
> 由此可以獲得樹狀結構 `("1")`

> 考慮一稍微複雜的運算式 `["1", "+", "1"]`
> 依序建立樹狀結構,
> 很顯然的第一個 `"1"` 將被放置於根節點 `("1")`,
> 接著, `"+"` 將取代 `"1"` 被放置於根節點, 左子樹指向原根節點的 `"1"`,
> 接著, `"1"` 將被放置於 `"+"` 的右子樹
> 由此可以獲得樹狀結構 `("+", ("1", "1"))`

> 考慮一更複雜的運算式 `["1", "+", "1", "+", "1", ...]`
> 依序建立樹狀結構, 參考上述的步驟, 可以獲得以下結論:
> 1. 如果根節點為空, 運算子放置於根節點; 否則運算子放置於最右節點的右子樹.
> 2. 運算符取代根節點, 並將原根節點設為左子樹.

> 考慮一更複雜的運算式`["1", "+", "1", "*", "1", ...]`
> 其中運算符的有優先度差異, 參考上面的結論,
> 依序建立樹狀結構, 可以獲得以下結論:
> 1. 如果根節點為空, 運算子放置於根節點; 否則運算子放置於最右節點的右子樹.
> 2. 如根節點為運算子, 運算符取代根節點, 並將原根節點設為左子樹.
> 3. 如根節點為運算符, 如果其優先度不小於新運算符, 新增行為同 (2.)
> 4. 如根節點為運算符, 如果其優先度小於新運算符,
> 取代原根節點的右子樹, 並將其左子樹指向原根節點的右子樹

> 基於上述結論, 對於僅使用基本四則運算的運算式,
> 已可成功建立其樹狀結構.

> 再來, 考慮有 `()` 的運算式,
> 以稍微一更複雜的運算式 `["1", "+", "(", "1", "+", "1", ")"]` 為例,
> 很顯然的, 我們可以將 `["(", "1", "+", "1", ")"]` 視為另一稍微簡單的運算式,
> 將其先行計算後取代原運算式中的位置, 而不影響整體的結果.

> 由此, 可以得出將根節點用一 stack 來加以維護,
> 遇到 `(` 就將當前根節點壓入 stack 並將當前最右節點的右子樹視為新根節點,
> 遇到 `)` 就將 stack 彈出, 並將彈出節點視為新根節點的結論.
> 為表示其優先度較 `+-*/` 還高, 在此引入暫時性符號 `(`, 並在函數尾端再將其移除.
> 至此, 樹狀結構構造完畢. 可以以此寫出生成函數

```c
// functional.h
void generate_tree(
	node** destination,
	node* source  // token-list
);
```

> 現在考慮所需輸出運算式結果的功能:

> 考慮最簡單的運算式: `("1")`,
> 訪問根節點即可得到答案.
> 再來, 考慮運算式: `("+", ("1", "1"))`,
> 很顯然的由於根節點是符號,
> 我們需要訪問其左右子樹查看其數值後, 才能再做計算獲得答案

> 由此類推可以構造出一遞迴函數,
> 當運算式為最簡單型態時 (純數值), 返回其結果.
> 當運算式非最簡單型態時, 求取其左右子樹的數值, 再做計算獲得結果.

```c
// functional.c
static float simplest_calculate(
	enum operator_type operator,
	float source_1,
	float source_2
);
float calculate(node* source)
{
	if (source->type == VALUE) return source->value;
	else return simplest_calculate(source->operator, calculate(source->left), calculate(source->right),);
}
```

> 最後, 考慮所需輸出 postfix 表達式之功能,
> 其即等同於輸出所構建的樹的 postfix

```c
// functional.c
void view_tree_postfix(node* source)
{
	if (source == NULL) return;
	view_tree_postfix(source->left);
	view_tree_postfix(source->right);
	print(source->data);
}
```


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



