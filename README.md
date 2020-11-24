# Data Structure - Homework 6: Calculator

## 目的(Goal)

> 以 C / C++ 語言實做一簡單計算機程式, 輸入 infix 表示式的數學運算式,
> 輸出運算式 postfix 表達式與其計算結果. 不保證輸入運算式合法, 需提供偵錯功能.

### 錯誤型態(Error Type)
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

## 分析(Anylize)
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
>> 找到一個方法, 將運算式拆分為一個符號跟一個後綴的運算式
> 以此為基礎, 可以構造第一個函數使得:
>> "1+(-1)" -> \["1", "+", "(", "-", 1", ")"\]

```c
// functional.h
void generate_token (
	node** destination,
	const char* source
);
```

> 接著, 考慮負數的表達式, 我們要將產生的 token-list 做一些預處理,
> 讓 \["(", "-", 1", ")"\] 變為 \["-1"\],
> 以此為目的, 構造出一個函數

```c
// functional.c
static void tree_remove_LPAR_OPT(
	node** destination
);
```
> 並將該函數加入前一函數尾部

> 接下來, 由於**不保證輸入運算式合法**, 構建一個語法檢查的函數,
> 以偵測給定錯誤型態列表中的錯誤, 並發出相關資訊.
> 為方便查找是否有錯誤發生, 同時配置的一個錯誤計數器與其相關函數.

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

> 如果語法檢查通過, 接下來考慮將生成的 token-list 轉為樹狀結構 (tree),

```c
// functional.h
void generate_tree(
	node** destination,
	node* source
);
```
> 到此, 剩下功能所需的基本功能都已完成.

> 現在考慮所需的, 輸出運算式結果的功能:

> 考慮最簡單的運算式: \("1"\),
> 訪問根節點即可得到答案.
> 再來, 考慮運算式: \("+", \("1", "1"\)\),
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

> 考慮所需的, 輸出 postfix 表達式之功能,
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



