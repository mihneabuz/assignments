if &cp | set nocp | endif
let s:cpo_save=&cpo
set cpo&vim
imap <S-Tab> <Plug>SuperTabBackward
inoremap <C-Tab> 	
inoremap <silent> <expr> <Plug>delimitMateS-BS delimitMate#WithinEmptyPair() ? "\<Del>" : "\<S-BS>"
inoremap <silent> <Plug>delimitMateBS =delimitMate#BS()
imap <F2>  ;set invpaste paste?
nmap  ;NERDTreeToggle
nnoremap : ;
nnoremap ; :
vmap [% [%m'gv``
vmap ]% ]%m'gv``
vmap a% [%v]%
vmap gx <Plug>NetrwBrowseXVis
nmap gx <Plug>NetrwBrowseX
vnoremap <silent> <Plug>NetrwBrowseXVis :call netrw#BrowseXVis()
nnoremap <silent> <Plug>NetrwBrowseX :call netrw#BrowseX(expand((exists("g:netrw_gx")? g:netrw_gx : '<cfile>')),netrw#CheckIfRemote())
nnoremap <F2> ;set invpaste paste?
inoremap  
imap 	 <Plug>SuperTabForward
inoremap  u
inoremap  u
let &cpo=s:cpo_save
unlet s:cpo_save
set autoindent
set autoread
set background=dark
set backspace=indent,eol,start
set completeopt=menuone,longest,preview
set fileencodings=ucs-bom,utf-8,default,latin1
set formatoptions=tcqrnlj
set helplang=en
set history=1000
set hlsearch
set incsearch
set laststatus=2
set listchars=tab:>\ ,trail:-,extends:>,precedes:<,nbsp:+
set modelines=0
set pastetoggle=<F2>
set path=.,/usr/include,,,**
set printoptions=paper:a4
set ruler
set runtimepath=~/.vim,~/.vim/plugged/nerdtree,~/.vim/plugged/syntastic,~/.vim/plugged/vim-gutentags,~/.vim/plugged/delimitMate.vim,~/.vim/plugged/vim-airline,~/.vim/plugged/jedi-vim,~/.vim/plugged/vim-airline-themes,~/.vim/plugged/supertab,~/.vim/plugged/vim-matlab,/var/lib/vim/addons,/etc/vim,~/.vim/plugged/vim-polyglot,/usr/share/vim/vim80,/usr/share/vim/vim80/pack/dist/opt/matchit,~/.vim/plugged/vim-polyglot/after,/etc/vim/after,/var/lib/vim/addons/after,~/.vim/plugged/jedi-vim/after,~/.vim/after
set sessionoptions=blank,buffers,curdir,folds,help,tabpages,winsize
set shiftwidth=4
set smarttab
set statusline=%{gutentags#statusline()}
set suffixes=.bak,~,.swp,.o,.info,.aux,.log,.dvi,.bbl,.blg,.brf,.cb,.ind,.idx,.ilg,.inx,.out,.toc
set noswapfile
set tabpagemax=50
set tabstop=4
set tags=./tags;,./TAGS,tags,TAGS
set textwidth=90
set ttimeout
set ttimeoutlen=100
set updatetime=300
set viewoptions=folds,cursor,curdir
set viminfo=!,'100,<9999,s100
set wildmenu
" vim: set ft=vim :
