# Haffman
A simple Haffman compressor and decompressor

## General Info

This is a simple Haffman compreesor and decompressor. Currently only support specifying both the input and output paths.  

Syntax:  
`hfm decode input output`  
`hfm encode input output`  
        
Also support starting the program and specifying the input and output at runtime one by one.  
Syntax:  
`encode input output`  
`decode input output`  
`exit`  

(`input` and `output` are file paths, respectively)

Notes:
1. Currently doen not support default file names.
2. Currently allows specifying arguments on multiple lines.
3. Currently does not support encoding multiple files. (When specifying files at runtime)
4. Currently cannot exit while specifying input and output.
5. Filename format has to be surrounded with one pair and one pair only of parenthesis at the outermost if seperators are presnt(line breaks excluded).(When specifying files at runtime)
6. May change the structure of header to make it smaller in the future. Backward compatibility not guaranteed (no version signature in encoded file by now)
7. May add c++17 dependency if feature goes on to include encoding directories.
