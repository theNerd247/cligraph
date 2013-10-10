if &cp | set nocp | endif
let s:cpo_save=&cpo
set cpo&vim
inoremap <silent> <C-Tab> =UltiSnips_ListSnippets()
xnoremap 	 :call UltiSnips_SaveLastVisualSelection()gvs
snoremap <silent> 	 :call UltiSnips_ExpandSnippet()
snoremap <silent> <NL> :call UltiSnips_JumpForwards()
snoremap <silent>  :call UltiSnips_JumpBackwards()
noremap ,n :call NoteCleaner#Notecleaner()  
nmap ,r :%s/\t/  /g
nmap ,u	 :s/\t// :nohlsearch 
nmap ,	 :s/^/\t/ :nohlsearch 
nmap ,T :!ctags --options="$HOME/.ctagsrc" 
nmap ,t :ptag 
nmap ,/ :nohlsearch
nmap ,q :q
nmap ,uc :s/^\/\///:nohlsearch
nmap ,uC 'a-dd:'a,'bs/^ \* /'b+dd:nohlsearch
nmap ,c :s/^/\/\//:nohlsearch
nmap ,C 'a^O/*:'a,'bs/^/ * /'b^o */:nohlsearch
nmap ,s :split ~/.vimrc
nmap ,w :w
nmap K :!firefox https://duckduckgo.com/?q= & 
nmap gx <Plug>NetrwBrowseX
nnoremap <silent> <Plug>NetrwBrowseX :call netrw#NetrwBrowseX(expand("<cWORD>"),0)
snoremap <silent> <BS> c
snoremap <silent> <C-Tab> :call UltiSnips_ListSnippets()
inoremap <silent> 	 =UltiSnips_ExpandSnippet()
inoremap <silent> <NL> =UltiSnips_JumpForwards()
inoremap <silent>  =UltiSnips_JumpBackwards()
imap  :read !xclip -o i
let &cpo=s:cpo_save
unlet s:cpo_save
set background=dark
set backspace=indent,eol,start
set completeopt=menuone
set fileencodings=ucs-bom,utf-8,default,latin1
set helplang=en
set ruler
set runtimepath=~/.vim,~/.vim/bundle/NoteCleaner,~/.vim/bundle/Project,~/.vim/bundle/UltiSnips,~/.vim/bundle/clang_complete,~/.vim/bundle/javacomplete,~/.vim/bundle/nerdtree,/usr/share/vim/vimfiles,/usr/share/vim/vim73,/usr/share/vim/vimfiles/after,~/.vim/bundle/UltiSnips/after,~/.vim/after
set showcmd
set suffixes=.bak,~,.swp,.o,.info,.aux,.log,.dvi,.bbl,.blg,.brf,.cb,.ind,.idx,.ilg,.inx,.out,.toc
set tabstop=2
set tags=./tags,./TAGS,tags,TAGS,/usr/include/tags
set textwidth=100
" vim: set ft=vim :
