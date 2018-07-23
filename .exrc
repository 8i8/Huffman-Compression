if &cp | set nocp | endif
let s:cpo_save=&cpo
set cpo&vim
imap <Nul> <C-Space>
inoremap <expr> <Up> pumvisible() ? "\" : "\<Up>"
inoremap <expr> <S-Tab> pumvisible() ? "\" : "\<S-Tab>"
inoremap <expr> <Down> pumvisible() ? "\" : "\<Down>"
inoremap <silent> <Plug>NERDCommenterInsert  <BS>:call NERDComment('i', "insert")
imap <F10> :TlistTogglei
imap <F8> :NERDTreeTogglei
imap <F6> ]s
imap <F5> [s
imap <F4> :setlocal spell!a
imap <C-=> <C-+>
imap <MiddleMouse> <Nop>
nmap d :cs find d =expand("<cword>")	
nmap i :cs find i ^=expand("<cfile>")$
nmap f :cs find f =expand("<cfile>")	
nmap e :cs find e =expand("<cword>")	
nmap t :cs find t =expand("<cword>")	
nmap c :cs find c =expand("<cword>")	
nmap g :cs find g =expand("<cword>")	
nmap s :cs find s =expand("<cword>")	
nnoremap  d :YcmShowDetailedDiagnostic
nnoremap <silent>  p :call conque_gdb#print_word(expand("<cword>"))
nnoremap <silent>  t :call conque_gdb#command("backtrace")
nnoremap <silent>  f :call conque_gdb#command("finish")
nnoremap <silent>  s :call conque_gdb#command("step")
nnoremap <silent>  n :call conque_gdb#command("next")
nnoremap <silent>  r :call conque_gdb#command("run")
nnoremap <silent>  c :call conque_gdb#command("continue")
nnoremap <silent>  b :call conque_gdb#toggle_breakpoint(expand("%:p"), line("."))
nmap  ca <Plug>NERDCommenterAltDelims
xmap  cu <Plug>NERDCommenterUncomment
nmap  cu <Plug>NERDCommenterUncomment
xmap  cb <Plug>NERDCommenterAlignBoth
nmap  cb <Plug>NERDCommenterAlignBoth
xmap  cl <Plug>NERDCommenterAlignLeft
nmap  cl <Plug>NERDCommenterAlignLeft
nmap  cA <Plug>NERDCommenterAppend
xmap  cy <Plug>NERDCommenterYank
nmap  cy <Plug>NERDCommenterYank
xmap  cs <Plug>NERDCommenterSexy
nmap  cs <Plug>NERDCommenterSexy
xmap  ci <Plug>NERDCommenterInvert
nmap  ci <Plug>NERDCommenterInvert
nmap  c$ <Plug>NERDCommenterToEOL
xmap  cn <Plug>NERDCommenterNested
nmap  cn <Plug>NERDCommenterNested
xmap  cm <Plug>NERDCommenterMinimal
nmap  cm <Plug>NERDCommenterMinimal
xmap  c  <Plug>NERDCommenterToggle
nmap  c  <Plug>NERDCommenterToggle
xmap  cc <Plug>NERDCommenterComment
nmap  cc <Plug>NERDCommenterComment
nmap <silent>  w t <Plug>VimwikiTabMakeDiaryNote
nmap <silent>  w w <Plug>VimwikiMakeDiaryNote
nmap <silent>  w i <Plug>VimwikiDiaryGenerateLinks
nmap <silent>  wi <Plug>VimwikiDiaryIndex
nmap <silent>  ws <Plug>VimwikiUISelect
nmap <silent>  wt <Plug>VimwikiTabIndex
nmap <silent>  ww <Plug>VimwikiIndex
nmap  tc <Plug>Colorizer
vmap  <F12> :w !curl -F 'f:1=<-' ix.io
nmap  <F12> :w !curl -F 'f:1=<-' ix.io
map  e :set nu!:set relativenumber!
map  a :set relativenumber!
map  q :set nu!
map  / :GitGutter 
map  m :GitGutterToggle
nmap  = :b12
nmap  - :b11
nmap  0 :b10
nmap  9 :b9
nmap  8 :b8
nmap  7 :b7
nmap  6 :b6
nmap  5 :b5
nmap  4 :b4
nmap  3 :b3
nmap  2 :b2
nmap  1 :b1
nmap  j <Down>
nmap  k <Up>
nmap  h <Left>
nmap  l <Right>
nmap  <Down> <Down>
nmap  <Up> <Up>
nmap  <Left> <Left>
nmap  <Right> <Right>
map <silent>  _ -
map <silent>  + +
vmap <silent>  - <
omap <silent>  - <
vmap <silent>  = >
omap <silent>  = >
nmap  \ A \j
nmap  z j0i<BS>j0i<BS>j0i<BS>j0i<BS>j0i<BS>j
imap µ →
imap  
vmap gx <Plug>NetrwBrowseXVis
nmap gx <Plug>NetrwBrowseX
vnoremap <silent> <Plug>NetrwBrowseXVis :call netrw#BrowseXVis()
nnoremap <silent> <Plug>NetrwBrowseX :call netrw#BrowseX(expand((exists("g:netrw_gx")? g:netrw_gx : '<cfile>')),netrw#CheckIfRemote())
nnoremap <silent> <F11> :call conque_term#exec_file()
xnoremap <silent> <Plug>NERDCommenterUncomment :call NERDComment("x", "Uncomment")
nnoremap <silent> <Plug>NERDCommenterUncomment :call NERDComment("n", "Uncomment")
xnoremap <silent> <Plug>NERDCommenterAlignBoth :call NERDComment("x", "AlignBoth")
nnoremap <silent> <Plug>NERDCommenterAlignBoth :call NERDComment("n", "AlignBoth")
xnoremap <silent> <Plug>NERDCommenterAlignLeft :call NERDComment("x", "AlignLeft")
nnoremap <silent> <Plug>NERDCommenterAlignLeft :call NERDComment("n", "AlignLeft")
nnoremap <silent> <Plug>NERDCommenterAppend :call NERDComment("n", "Append")
xnoremap <silent> <Plug>NERDCommenterYank :call NERDComment("x", "Yank")
nnoremap <silent> <Plug>NERDCommenterYank :call NERDComment("n", "Yank")
xnoremap <silent> <Plug>NERDCommenterSexy :call NERDComment("x", "Sexy")
nnoremap <silent> <Plug>NERDCommenterSexy :call NERDComment("n", "Sexy")
xnoremap <silent> <Plug>NERDCommenterInvert :call NERDComment("x", "Invert")
nnoremap <silent> <Plug>NERDCommenterInvert :call NERDComment("n", "Invert")
nnoremap <silent> <Plug>NERDCommenterToEOL :call NERDComment("n", "ToEOL")
xnoremap <silent> <Plug>NERDCommenterNested :call NERDComment("x", "Nested")
nnoremap <silent> <Plug>NERDCommenterNested :call NERDComment("n", "Nested")
xnoremap <silent> <Plug>NERDCommenterMinimal :call NERDComment("x", "Minimal")
nnoremap <silent> <Plug>NERDCommenterMinimal :call NERDComment("n", "Minimal")
xnoremap <silent> <Plug>NERDCommenterToggle :call NERDComment("x", "Toggle")
nnoremap <silent> <Plug>NERDCommenterToggle :call NERDComment("n", "Toggle")
xnoremap <silent> <Plug>NERDCommenterComment :call NERDComment("x", "Comment")
nnoremap <silent> <Plug>NERDCommenterComment :call NERDComment("n", "Comment")
nmap <Nul><Nul>d :vert scs find d =expand("<cword>")
nmap <Nul><Nul>i :vert scs find i ^=expand("<cfile>")$	
nmap <Nul><Nul>f :vert scs find f =expand("<cfile>")	
nmap <Nul><Nul>e :vert scs find e =expand("<cword>")
nmap <Nul><Nul>t :vert scs find t =expand("<cword>")
nmap <Nul><Nul>c :vert scs find c =expand("<cword>")
nmap <Nul><Nul>g :vert scs find g =expand("<cword>")
nmap <Nul><Nul>s :vert scs find s =expand("<cword>")
nmap <Nul>d :scs find d =expand("<cword>")	
nmap <Nul>i :scs find i ^=expand("<cfile>")$	
nmap <Nul>f :scs find f =expand("<cfile>")	
nmap <Nul>e :scs find e =expand("<cword>")	
nmap <Nul>t :scs find t =expand("<cword>")	
nmap <Nul>c :scs find c =expand("<cword>")	
nmap <Nul>g :scs find g =expand("<cword>")	
nmap <Nul>s :scs find s =expand("<cword>")	
nnoremap <silent> <Plug>Colorizer :ColorToggle
nmap <C-Â>� o[//]: # (  )ok$hha
vmap <F10> :TlistToggle
nmap <F10> :TlistToggle
vmap <F8> :NERDTreeToggle
nmap <F8> :NERDTreeToggle
nmap <F7> z=
vmap <F6> "+y:!duck "
nmap <F6> ]s
nmap <F5> [s
nmap <F4> :setlocal spell!
vmap <F3> "+y:grep -r " *:copen:noh
nmap <F3> :grep -R "TODO NEXT" *:copen:noh
vmap <F2> y :grep -R """ *:copen:noh
nmap <F2> :grep -R "TODO NOW" *:copen:noh
vmap <F1> y :grep -R """ *:copen:noh
nmap <F1> :grep -R TODO *:copen:noh
nmap <C-=> <C-+>
map <MiddleMouse> <Nop>
inoremap <expr> 	 pumvisible() ? "\" : "\	"
inoremap ( (:silent call HiCursor()a
inoremap ) ):call MatchedBracket()a
nmap £ o[//]: # () remark -  )ok$hha
nmap  
vmap  
inoremap [ [:silent call HiCursor()a
inoremap ] ]:call MatchedBracket()a
inoremap { {:silent call HiCursor()a
inoremap } }:call MatchedBracket()a
iabbr Yagya Yājya
iabbr yagya yājya
iabbr yadavas yādavas
iabbr vyayesha vyayeśa
iabbr vritti vṛtti
iabbr vrishika vṛścika
iabbr Vrishicka Vṛścika
iabbr vrishicka vṛścika
iabbr Vrishica Vṛścika
iabbr Vrishaka Vṛścika
iabbr vrishaka vṛścika
iabbr Vrishabha Vṛṣabha
iabbr vrishabha vṛṣabha
iabbr Vrishaba Vṛṣabha
iabbr vrishaba Vṛṣabha
iabbr vrishchika vṛścik
iabbr Vrischika Vṛścika
iabbr vrischika vṛścika
iabbr vrishcika vṛścika
iabbr Vrishcika Vṛścika
iabbr Vrishakaha vṛścika
iabbr vrishakaha vṛścika
iabbr Vrischicka Vṛścika
iabbr vrischicka vṛścika
iabbr Vrishakha Vṛścika
iabbr vrishakha vṛścika
iabbr Vishswamitra Viśvāmitra
iabbr vishswamitra viśvāmitra
iabbr Vishnu Viṣṇu
iabbr vishnu viṣṇu
iabbr viprarita viparīta
iabbr viparita viparīta
iabbr Vimsottary Vimśottari
iabbr vimsottary vimśottari
iabbr Vimsottari Vimśottari
iabbr vimsottari vimśottari
iabbr Vimsottarie Vimśottari
iabbr vimsottarie vimśottari
iabbr Vimshaottary Vimśottari
iabbr vimshaottary vimśottari
iabbr VidyA Vidyā
iabbr vidyA vidyā
iabbr vidya vidyā
iabbr vinyasa vinyāsa
iabbr vina vīṇā
iabbr veena vīṇā
iabbr Vedanga Vedāṅga
iabbr vedanga vedāṅga
iabbr Vashishta Vasiṣṭha
iabbr vashishta vasiṣṭha
iabbr vashista vaśiṣṭha
iabbr Vashista Vaśiṣṭha
iabbr Vayu Vāyu
iabbr vayu vāyu
iabbr Vaysha Vaiśya
iabbr vaysha vaiśya
iabbr Vatsyana Vātsyāyana
iabbr vatsyana vātsyāyana
iabbr Vata Vāta
iabbr vata vāta
iabbr Varuna Varuṇa
iabbr varuna varuṇa
iabbr varaha varāha
iabbr Varaha Varāha
iabbr Vamana Vāmana
iabbr Uttarashadha Uttarāṣāḍha
iabbr uttarashadha uttarāṣāḍha
iabbr Upadesha Upadeśa
iabbr upadesha upadeśa
iabbr uccA uccā
iabbr ucca uccā
iabbr twelth twelfth
iabbr Tula Tulā
iabbr tula tulā
iabbr Trikona Trikoṇa
iabbr trikona trikoṇa
iabbr Tripurasundarie Tripurāsundarī
iabbr tripurasundarie tripurāsundarī
iabbr trishul triśūl
iabbr trishula triśūl
iabbr tould told
iabbr THey They
iabbr teh the
iabbr THe The
iabbr THat That
iabbr tara tāra
iabbr Tandava Tāṇḍava
iabbr tandava tāṇḍava
iabbr tamoguna tamoguṇa
iabbr Swashetra Swaśetra
iabbr swashetra swaśetra
iabbr SwakAraka Swakāraka
iabbr Swakaraka Swakāraka
iabbr swakAraka swakāraka
iabbr swakaraka swakāraka
iabbr Sutra Sūtra
iabbr sutra sūtra
iabbr sutesha suteśa
iabbr Sutesha Suteśa
iabbr Surya Sūrya
iabbr surya sūrya
iabbr Sukesha Sukheśa
iabbr sukesha sukheśa
iabbr Sukra Śukra
iabbr sukra śukra
iabbr sthana sthāna
iabbr stri strī
iabbr Sri Śrī
iabbr sri śrī
iabbr soFt saumya
iabbr Sloka Śloka
iabbr sloka śloka
iabbr Sivaya Śivāya
iabbr sivaya śivāya
iabbr Siva Śiva
iabbr siva śiva
iabbr sishya śiṣya
iabbr Sisya Śiṣya
iabbr sisya śiṣya
iabbr Simha Siṃha
iabbr simha siṃha
iabbr Simhasana Siṁhāsana
iabbr simhasana siṁhāsana
iabbr shuba śubha
iabbr Shunya Śūnya
iabbr shunya śūnya
iabbr Shukra Śukra
iabbr shukra śukra
iabbr Shudra Śūdra
iabbr shudra śūdra
iabbr shuberkartari śubhakartarī
iabbr Shubha Śubha
iabbr shubha śubha
iabbr shloka śloka
iabbr shlim ślīṁ
iabbr Shiva Śiva
iabbr shiva śiva
iabbr Shastra Śāstra
iabbr Shani Śani
iabbr shani śani
iabbr shakshi sākṣī
iabbr Shakti Śakti
iabbr shakti śakti
iabbr shatryia kṣatriya
iabbr shatrya kṣatriya
iabbr shadbala ṣaḍbala
iabbr Shadbala Ṣaḍbala
iabbr SaDakSara ṣaḍakṣara
iabbr subrahmanya subrahmaṇya
iabbr Subrahmanya Subrahmaṇya
iabbr Somya Saumya
iabbr somya saumya
iabbr sashthesha ṣaṣṭheśa
iabbr sashtesha ṣaṣṭheśa
iabbr Sashtesha Ṣaṣṭheśa
iabbr sasha śaśá
iabbr Saraswati Sarasvatī
iabbr saraswati sarasvatī
iabbr spouce spouse
iabbr samyasi saṁnyāsī
iabbr samyasa saṁnyāsa
iabbr Samyas Saṁnyāsa
iabbr samyas saṁnyāsa
iabbr Sanyasa Saṁnyāsa
iabbr sanyasa saṁnyāsa
iabbr Sanyas Saṁnyāsa
iabbr sanyas saṁnyāsa
iabbr shankaracharya śaṅkarācārya
iabbr Shankaracharya Śaṅkarācārya
iabbr sankaracharyas śaṅkarācārya
iabbr Sankaracharyas Śaṅkarācārya
iabbr Shankara Śaṅkara
iabbr shankara śaṅkara
iabbr Sankara Śaṅkara
iabbr sankara śaṅkara
iabbr Sanis Śani's
iabbr sanis śani's
iabbr Sani Śani
iabbr sani śani
iabbr Sancranti Saṁkrānti
iabbr sancranti saṁkrānti
iabbr Sankranti Saṁkrānti
iabbr sankranti saṁkrānti
iabbr Samyoga Saṁyoga
iabbr samyoga saṁyoga
iabbr Samsara Saṃsāra
iabbr samsara saṃsāra
iabbr samadhi samādhi
iabbr samedhi samādhi
iabbr samedi samādhi
iabbr Sakti Śakti
iabbr sakti śakti
iabbr Sahasrara Sahasrāra
iabbr sahasrara sahasrāra
iabbr sasthesa ṣaṣṭheśa
iabbr Sasthesa Ṣaṣṭheśa
iabbr shatru śatru
iabbr sadhana sādhana
iabbr Shadripu Ṣaḍripu
iabbr shadripu ṣaḍripu
iabbr sadripu ṣaḍripu
iabbr sadhu sādhu
iabbr Sadashiva Sadāśiva
iabbr sadashiva sadāśiva
iabbr Shivaya Śivāya
iabbr shivaya śivaya
iabbr Shashtiamsa Saṣṭyāṁśa
iabbr shashtiamsa ṣaṣṭyāṁśa
iabbr shastra śāstra
iabbr Shastiamsa Saṣṭyāṁśa
iabbr shastiamsa ṣaṣṭyāṁśa
iabbr Shanti Śānti
iabbr shanti śānti
iabbr Sashtiamsa Ṣaṣṭyāṁśa
iabbr sashtiamsa ṣaṣṭyāṁśa
iabbr sashtiamsha ṣaṣṭyāṁśa
iabbr sanskritam saṃskṛtam
iabbr sanskritum saṃskṛtam
iabbr samscara saṃskāra
iabbr samvatsara sāṃvatsara
iabbr DD60 ṣaṣṭyāṁśa
iabbr Rohini rohiṇī
iabbr rohini rohiṇī
iabbr rtu ṛtu
iabbr rishikesha ṛṣīkeśa
iabbr rishikesh ṛṣīkeśa
iabbr rista riṣṭa
iabbr rishta riṣṭa
iabbr rishi ṛṣi
iabbr reitious righteous
iabbr risi ṛṣi
iabbr rsi ṛṣi
iabbr rudraksha rudrākṣa
iabbr Ropi Ropī
iabbr Rna Ṛṇa
iabbr rna ṛṇa
iabbr Ravana Rāvaṇa
iabbr ravana rāvaṇa
iabbr ratri rātri
iabbr ratra rātra
iabbr rati ratī
iabbr rashi rāśi
iabbr Rashi Rāśi
iabbr Rasi Rāśi
iabbr rasi rāśi
iabbr Ramakrishna Rāmakṛṣṇa
iabbr ramakrishna rāmakṛṣṇa
iabbr RAma Rāma
iabbr Rama Rāma
iabbr rAma rāma
iabbr rama rāma
iabbr Rajayoga Rājayoga
iabbr rajayoga rājayoga
iabbr Rakshasa Rkkṣas
iabbr rakshasa rākṣas
iabbr Raksha Rakṣa
iabbr raksha rakṣa
iabbr Raksasa Rākṣas
iabbr raksasa rākṣas
iabbr Raksa Rakṣa
iabbr raksa rakṣa
iabbr Raja Rāja
iabbr raja rāja
iabbr Rahus Rāhus
iabbr rahus rāhus
iabbr Rahu Rāhu
iabbr rahu rāhu
iabbr Pushya Puṣya
iabbr pushya puṣya
iabbr purna pūrṇa
iabbr Purna Pūrṇa
iabbr Purana Purāṇa
iabbr purana purāṇa
iabbr purusha puruṣa
iabbr perusha puruṣa
iabbr perculiar peculiar
iabbr pujita pūjita
iabbr puja pūjā
iabbr prinsiple principle
iabbr priti prīti
iabbr Prithvi Pṛthvī
iabbr prithvi pṛthvī
iabbr pratipada prātipada
iabbr prashna praśna
iabbr prakrit prākṛta
iabbr Prajapati Prajāpati
iabbr prajapati prajāpati
iabbr pranama praṇāma
iabbr pranam pranāma
iabbr pranas prāṇas
iabbr prana prāṇa
iabbr pournima pūrṇimā
iabbr Pournima Pūrṇimā
iabbr Placment Placement
iabbr placment placement
iabbr Pithris Pitṛis
iabbr pithris pitṛis
iabbr Pitris Pitṛis
iabbr pitris pitṛis
iabbr Pitri Pitṛ
iabbr pitri pitṛ
iabbr Pischarch Piśāca
iabbr pischarch piśāca
iabbr Picarcher Piśāca
iabbr picarcher piśāca
iabbr Pisatcha Piśāca
iabbr pisatcha piśāca
iabbr Piscatcha Piśāca
iabbr piscatcha piśāca
iabbr Parvarti Pārvatī
iabbr parvarti pārvatī
iabbr parkalagna pāka lagna
iabbr Parashar Pārāśara
iabbr parashar pārāśara
iabbr Pararashar Pārāśara
iabbr pararashar pārāśara
iabbr PArAshara Pārāśara
iabbr Parashara Pārāśara
iabbr pArAshara Pārāśara
iabbr parashara pārāśara
iabbr Parasara Pārāśara
iabbr parasara pārāśara
iabbr parampara paramparā
iabbr paramparA paramparā
iabbr Parakrama Parākrama
iabbr parakrama parākrama
iabbr pApa pāpa
iabbr PApakartarI Pāpakartarī
iabbr Papakartari Pāpakartarī
iabbr papakartari pāpakartarī
iabbr Panini Pāṇini
iabbr panini pāṇini
iabbr Panapara Paṇapara
iabbr panapara paṇapara
iabbr panchakshari pañcākṣarī
iabbr panchaksari pañcākṣarī
iabbr panshakshara pañcākṣara
iabbr panchatattva pañcatattva
iabbr pancharatra pañcarātra
iabbr Panchanga Pañcāṅga
iabbr panchanga pañcāṅga
iabbr Panchang Pañcāṅga
iabbr panchang pañcāṅga
iabbr Pakesha Pākaeśa
iabbr pakesha pākaeśa
iabbr pakalagna pākalagna
iabbr pAka pāka
iabbr paka pāka
iabbr Om Oṁ
iabbr om oṁ
iabbr Orissa Oṛiśā
iabbr orissa oṛiśā
iabbr oRishA oṛiśā
iabbr nyasa nyāsa
iabbr Nipuna Nipuṇa
iabbr nipuna nipuṇa
iabbr nijadosha nijadoṣa
iabbr Nichabhanga Nīcabhañga
iabbr nichabhanga nīcabhañga
iabbr Nichabanga Nīcabhañga
iabbr nichabanga nīcabhañga
iabbr Nica Nīca
iabbr nica nīca
iabbr Navamsha Navāṁśa
iabbr navamsha navāṁśa
iabbr Navamsa Navāṁśa
iabbr navamsa navāṁśa
iabbr Navamasa Navāṁśa
iabbr navamasa navāṁśa
iabbr narayana nārāyaṇa
iabbr Narayana Nārāyaṇa
iabbr Naryana Nārāyaṇa
iabbr naryana nārāyaṇa
iabbr Narashimhaya Narasiṁhāya
iabbr narashimhaya narasiṁhāya
iabbr Narasimha Nṛasiṁha
iabbr narasimha nṛasiṁha
iabbr Narka Nāraka
iabbr narka nāraka
iabbr namah namaḥ
iabbr Nakshatra Nakṣatra
iabbr nakshatra nakṣatra
iabbr Naksatra Nakṣatra
iabbr naksatra nakṣatra
iabbr nAga nāga
iabbr naga nāga
iabbr nagamuni nāgamuni
iabbr nagamani nāga-mani
iabbr Mulatrikona Mūlatrikoṇa
iabbr mulatrikona mūlatrikoṇa
iabbr mulahadara mūlādhāra
iabbr Muladhara Mūlādhāra
iabbr muladhara mūlādhāra
iabbr Mulhadara Mūlādhāra
iabbr mulhadara mūlādhāra
iabbr Muladara Mūlādhāra
iabbr muladara mūlādhāra
iabbr Mula Mūla
iabbr mula mūla
iabbr muhUrta muhūrta
iabbr muhurta muhūrta
iabbr Mritunjaya Mṛtyuñjaya
iabbr mritunjaya mṛtyuñjaya
iabbr mritue mṛtyu
iabbr Mritu Mṛtyu
iabbr mritu mṛtyu
iabbr Mrtyu Mṛtyu
iabbr mrtyu mṛtyu
iabbr mrytryu mṛtyu
iabbr Mridu Mṛdu
iabbr mridu mṛdu
iabbr Mrigrisira Mṛgaśiras
iabbr mrigrisira mṛgaśiras
iabbr Migrisira Mṛgaśiras
iabbr migrisira mṛgaśiras
iabbr Moolatrikona Mūlatrikoṇa
iabbr moolatrikona mūlatrikoṇa
iabbr Moksha Mokṣa
iabbr moksha mokṣa
iabbr Moksa Mokṣa
iabbr moksa mokṣa
iabbr MKS maraṇa kāraka sthāna
iabbr mks maraṇa kāraka sthāna
iabbr Mituna Mithuna
iabbr mituna mithuna
iabbr Mina Mīna
iabbr mina mīna
iabbr Mimamsa Mīmāṃsā
iabbr mimamsa mīmāṃsā
iabbr Mesha Meśa
iabbr mesha meśa
iabbr Meena Mīna
iabbr meena mīna
iabbr matreya mātrīya
iabbr Matri Mātṛ
iabbr matri mātṛ
iabbr masa māsa
iabbr Maruta Māruta
iabbr maruta māruta
iabbr mantresha mantreśa
iabbr Mangala Maṅgala
iabbr mangala maṅgala
iabbr Mangal Maṅgal
iabbr mangal maṅgal
iabbr manisia manīṣā
iabbr malific malefic
iabbr Mahurta Muhūrta
iabbr mahurta muhūrta
iabbr mahourta muhūrta
iabbr mahavidya mahāvidyā
iabbr Mahatma Mahātmā
iabbr mahatma mahātmā
iabbr Mahasaraswati Mahāsarasvatī
iabbr mahasaraswati mahāsarasvatī
iabbr Mahasarasvati Mahāsarasvatī
iabbr mahasarasvati mahāsarasvatī
iabbr maharsi maharṣi
iabbr maharshi maharṣi
iabbr maharishi maharṣi
iabbr Maharaja Mahārāja
iabbr maharaja mahārāja
iabbr mahapurusha mahāpuruṣa
iabbr mahaperusha mahāpuruṣa
iabbr Mahalaximi Mahālakṣmī
iabbr mahalaximi mahālakṣmī
iabbr mahalaxshmi mahālakṣmī
iabbr Mahakali Mahākālī
iabbr mahakali mahākālī
iabbr maha mahā
iabbr Linga Liṅga
iabbr linga liṅga
iabbr LLL Lagneśa
iabbr lll lagneśa
iabbr Laxshmi Lakṣmī
iabbr Laxshimi Lakṣmī
iabbr laxshimi lakṣmī
iabbr Laximi Lakṣmī
iabbr Laxmi Lakṣmī
iabbr laxshmi lakṣmī
iabbr laximi lakṣmī
iabbr laxmi lakṣmī
iabbr Lagnesha Lagneśa
iabbr lagnesha lagneśa
iabbr Lagnamsha Lagnāṁśa
iabbr lagnamsha lagnāṁśa
iabbr Labha Lābha
iabbr labha lābha
iabbr Labesha Lābheśa
iabbr labesha lābheśa
iabbr kutumbha kuṭumba
iabbr kumari kumār
iabbr Kurma Kūrma
iabbr kurma kūrma
iabbr Kundalini Kuṇḍalinī
iabbr kundalini kuṇḍalinī
iabbr Kshatryia Kṣatriya
iabbr kshatryia kṣatriya
iabbr Krura Krūra
iabbr krura krūra
iabbr Kritika Kṛttikā
iabbr Krittika Kṛttikā
iabbr kritika kṛttikā
iabbr krittika kṛttikā
iabbr Krishna Kṛṣṇa
iabbr krishna Kṛṣṇa
iabbr kripa kṛpā
iabbr Kona Koṇa
iabbr kona koṇa
iabbr Klim Klīṁ
iabbr klim klīṁ
iabbr kavi kavī
iabbr Kashyap Kaśyapa
iabbr kashyap kaśyapa
iabbr Karna Karṇa
iabbr Karyesha Kāryeśa
iabbr karyesha kāryeśa
iabbr KArya Kārya
iabbr Karya Kārya
iabbr kArya kārya
iabbr karya kārya
iabbr kartikayer kārtikeya
iabbr Kartika Kārtika
iabbr kartika kārtika
iabbr karmesha karmeśa
iabbr Karmesha Karmeśa
iabbr KAra Kāra
iabbr Kara Kāra
iabbr kAra kāra
iabbr kara kāra
iabbr KAraka Kāraka
iabbr Karaka Kāraka
iabbr kAraka kāraka
iabbr karaka kāraka
iabbr Kanya Kanyā
iabbr kanya kanyā
iabbr kantaka kaṇṭaka
iabbr KAma Kāma
iabbr Kama Kāma
iabbr kAma kāma
iabbr kama kāma
iabbr Kali Kālī
iabbr kali kālī
iabbr jyotirlinga jyotirliṅga
iabbr Jyotisi Jyotiṣī
iabbr jyotisi jyotiṣī
iabbr Jyotish Jyotiṣa
iabbr jyotish jyotiṣa
iabbr Jyotishi Jyotiṣī
iabbr jyotshi jyotiṣī
iabbr jyotishi jyotiṣī
iabbr Jyotisha Jyotiṣa
iabbr jyotisha jyotiṣa
iabbr jyeshtha jyeṣṭha
iabbr jnanendriyas jñānendriyas
iabbr jnayna jñāna
iabbr jana jñāna
iabbr Jnana Jñāna
iabbr jnana jñāna
iabbr Jivatman Jīvātman
iabbr jivatman jīvātman
iabbr jiva jīva
iabbr Jati Jāti
iabbr jati jāti
iabbr JAtaka Jātaka
iabbr Jataka Jātaka
iabbr jAtaka jātaka
iabbr jataka jātaka
iabbr Jalatman Jalātman
iabbr jalatman jalātman
iabbr Jala Jalā
iabbr jala jalā
iabbr Ishta Iṣṭa
iabbr ishta iṣṭa
iabbr Ishana Īśāna
iabbr ishana īśāna
iabbr isha iṣa
iabbr Indrani Indrāṇī
iabbr indrani indrāṇī
iabbr iccha iccā
iabbr icha iccā
iabbr icca iccā
iabbr hrdroga hṛdroga
iabbr hidroga hṛdroga
iabbr hte the
iabbr horascope horāscope
iabbr Hora Horā
iabbr hora horā
iabbr hosue house
iabbr Hrshikesha Hṛṣikeśa
iabbr hrshikesha hṛṣikeśa
iabbr Haraya Hārāya
iabbr haraya hārāya
iabbr Hanuman Hanumān
iabbr hanuman Hanumān
iabbr gurumangala gurumaṅgala
iabbr Guna Guṇa
iabbr guna guṇa
iabbr Grahana Grahaṇa
iabbr grahana grahaṇa
iabbr gopi gopī
iabbr gotra gotrā
iabbr gotera gotrā
iabbr gottera gotrā
iabbr Gita Gītā
iabbr gita gītā
iabbr Ghatika Ghāṭikā
iabbr ghatika ghāṭikā
iabbr gayatri gāyatrī
iabbr giatry gāyatrī
iabbr Gauri Gaurī
iabbr gauri gaurī
iabbr Ganesh Gaṇeśa
iabbr ganesh gaṇeśa
iabbr Ganesha Gaṇeśa
iabbr ganesha gaṇeśa
iabbr Gandantia Gaṇḍānta
iabbr gantantia gaṇḍānta
iabbr gandanta gaṇḍānta
iabbr gandantia gaṇḍānta
iabbr gajakeshary gajakeśarī
iabbr gagacheshari gajakeśarī
iabbr gajakeshari gajakeśarī
iabbr gambeling gambling
iabbr ecadashi ekādaśī
iabbr ekardasha ekādaśa
iabbr ekardasa ekādaśa
iabbr ekadasha ekādaśa
iabbr engagment engagement
iabbr Dwardashamsha Dvādaśāṁśa
iabbr dwardashamsha dvādaśāṁśa
iabbr Duscikya Duścikya
iabbr duscikya duścikya
iabbr Dushkarma Duṣkarma
iabbr dushkarma duṣkarma
iabbr Drkbala Dṛgbala
iabbr drkbala dṛgbala
iabbr Dristi Dṛṣṭi
iabbr dristi dṛṣṭi
iabbr Drishti Dṛṣṭi
iabbr drishti dṛṣṭi
iabbr Drish Dṛś
iabbr drish dṛś
iabbr Dris Dṛś
iabbr dris dṛś
iabbr DRg Dṛg
iabbr Drg Dṛg
iabbr dRg dṛg
iabbr drg dṛg
iabbr Drgbala Dṛgbala
iabbr drgbala dṛgbala
iabbr drekkana drekkāṇa
iabbr Dosha Doṣa
iabbr dosha doṣa
iabbr Dosa Doṣa
iabbr dosa doṣa
iabbr dipvali dīpāvali
iabbr divali dīpāvali
iabbr Diksha Dīkṣā
iabbr diksha dīkṣā
iabbr DigpAla Digpāla
iabbr Digpala Digpāla
iabbr digpAla digpāla
iabbr digpala digpāla
iabbr Dhi Dhī
iabbr dhi dhī
iabbr dhatu dhātu
iabbr dharmaesha dhārmeśa
iabbr Dharmesha Dhārmeśa
iabbr dharmesha dhārmeśa
iabbr Dhanesha Dhaneśa
iabbr dhanesha dhaneśa
iabbr DhArma Dhārma
iabbr Dharma Dhārma
iabbr dhArma dhārma
iabbr dharma dhārma
iabbr definatly definitely
iabbr Devi Devī
iabbr devi devī
iabbr DevatA Devatā
iabbr devatA devatā
iabbr devanagri devanāgarī
iabbr danda daṇḍa
iabbr DayA Dayā
iabbr Daya Dayā
iabbr dayA dayā
iabbr daya dayā
iabbr Dashavarga Daśavarga
iabbr dashavarga daśavarga
iabbr Dashamsha Dasāṁśa
iabbr dashamsha dasāṁśa
iabbr Dashamsa Dasāṁśa
iabbr dashamsa dasāṁśa
iabbr dasha dāśa
iabbr Dasavarga Daśavarga
iabbr dasavarga daśavarga
iabbr dasa dāśa
iabbr Darshamsha Dasāṁśa
iabbr darshamsha dasāṁśa
iabbr Darshamsa Dasāṁśa
iabbr darshamsa dasāṁśa
iabbr daresha dāreśa
iabbr Daresha Dāreśa
iabbr Daksina Dakṣiṇā
iabbr daksina dakṣiṇā
iabbr d60C Ṣaṣṭiāṁśa
iabbr d60c ṣaṣṭiāṁśa
iabbr curtious courteous
iabbr chandrastama chandraṣṭama
iabbr chaturashteri caturakṣari
iabbr chaturashra caturaśra
iabbr chandrabhaga candrabhāgā
iabbr Charakyas Cāṇakya
iabbr charakyas cāṇakya
iabbr chestabala ceṣṭābala
iabbr Chaymunda Cāmuṇḍā
iabbr chamunda cāmuṇḍā
iabbr chaymunda cāmuṇḍā
iabbr bhrigu bhṛgu
iabbr brigu bhṛgu
iabbr Bhrigu Bhṛgu
iabbr Brigu Bhṛgu
iabbr Brihat Bṛhat
iabbr brihat bṛhat
iabbr brhaspati bṛhaspati
iabbr Brhaspati Bṛhaspati
iabbr bhramhacharya bramhacharya
iabbr bhramans brāhmaṇs
iabbr bhramins brāhmiṇs
iabbr bhraminhas brāhmiṇa
iabbr Bhramhana Brāhmaṇa
iabbr bhramhana brāhmaṇa
iabbr Brahmana Brāhmaṇa
iabbr brahmana brāhmaṇa
iabbr Bramhanya Brahmaṇya
iabbr bramhanya brahmaṇya
iabbr Bhima Bhīma
iabbr bhima bhīma
iabbr Bhema Bhīma
iabbr bhema bhīma
iabbr Beema Bhīma
iabbr beema bhīma
iabbr bhu hū
iabbr Bhu Bhū
iabbr buisnessman businessman
iabbr buisness business
iabbr Bhavesha Bhaveśa
iabbr bhavesha bhaveśa
iabbr bhavam bhāvaṁ
iabbr BhAva Bhāva
iabbr Bhava Bhāva
iabbr bhAva bhāva
iabbr Bharanini Bharaṇī
iabbr bharanini bharaṇī
iabbr bhava bhāva
iabbr bhagyesha bhāgyeśe
iabbr Bhagyesha Bhāgyeśe
iabbr Bhagya Bhāgya
iabbr bhagya bhāgya
iabbr bhadakesha bādhakeśa
iabbr bhachack bhāṣaka
iabbr bb bhavat-bhāvaṁ
iabbr bala bāla
iabbr bahadakesh bādhakeśa
iabbr Badhak Bādhaka
iabbr badhak bādhaka
iabbr BAdhaka Bādhaka
iabbr Badhaka Bādhaka
iabbr bAdhaka bādhaka
iabbr badhaka bādhaka
iabbr badakesh bādhakeśa
iabbr BAdak Bādhaka
iabbr Badak Bādhaka
iabbr bAdak bādhaka
iabbr badak bādhaka
iabbr Ayush Āyus
iabbr ayush āyus
iabbr Ayus āyus
iabbr ayus āyus
iabbr aynamsha ayanāṃśa
iabbr Atman ātman
iabbr AtmakAraka Ātmakāraka
iabbr Atmakaraka Ātmakāraka
iabbr atmakaraka ātmakāraka
iabbr atmakraka ātmakāraka
iabbr AtmA ātmā
iabbr Atma ātma
iabbr Asta Aṣṭa
iabbr asta aṣṭa
iabbr astamesha aṣṭameśa
iabbr Astamesha Aṣṭameśa
iabbr ashtottary aṣṭottarī
iabbr ashtakshari aṣṭākṣari
iabbr ashtanga aṣṭāṅga
iabbr Ashtama Aṣṭama
iabbr ashtama aṣṭama
iabbr ashwini aśvinī
iabbr Ashwini Aśvinī
iabbr Ashvini Aśvinī
iabbr ashvini aśvinī
iabbr Ashoka Aśoka
iabbr ashoka aśoka
iabbr arudha ārūḍha
iabbr archarya arcārha
iabbr anustubh anuṣṭubh
iabbr Anustubh Anuṣṭubh
iabbr amrita amṛta
iabbr Amavasya Amāvasyā
iabbr amavasya amāvasyā
iabbr amsha āṁśa
iabbr Allways Always
iabbr allways always
iabbr Akshari Ākṣari
iabbr akshari ākṣari
iabbr Akshara Akṣara
iabbr akshara akṣara
iabbr Akash Ākāśa
iabbr akash ākāśa
iabbr Akasa Ākāśa
iabbr akasa ākāśa
iabbr ahoratra ahorātra
iabbr ahimsa ahiṃsā
iabbr Ahamkar Ahaṃkāra
iabbr ahamkar ahaṃkāra
iabbr Adharma Adhārma
iabbr adharma adhārma
iabbr Aditya Āditya
iabbr aditya āditya
iabbr abichara abhicāra
iabbr 9thL Bhāgyeśa
iabbr 9thl bhāgyeśa
iabbr 9thH Dhārma bhāva
iabbr 9thh dhārma bhāva
iabbr 8thL Aṣṭameśa
iabbr 8thl aṣṭameśa
iabbr 8thH Randhra bhāva
iabbr 8thh randhra bhāva
iabbr 7thL Dāreśa
iabbr 7thl dāreśa
iabbr 7thH Jāyā bhāva
iabbr 7thh jāyā bhāva
iabbr 6thL Ṣaṣṭeśa
iabbr 6thl ṣaṣṭeśa
iabbr 6thH Ṣaṣṭha bhāva
iabbr 6thh ṣaṣṭha bhāva
iabbr 5thL Suteśa
iabbr 5thl suteśa
iabbr 5thH Putra bhāva
iabbr 5thh putra bhāva
iabbr 4thL Sukheśa
iabbr 4thl sukheśa
iabbr 4thH Mātṛ bhāva
iabbr 4thh mātṛ bhāva
iabbr 3rdL Sahajeśa
iabbr 3rdl sahajeśa
iabbr 3rdH Duścikya bhāva
iabbr 3rdh duścikya bhāva
iabbr 2ndL Dhaneśa
iabbr 2ndl dhaneśa
iabbr 2ndH Dhana bhāva
iabbr 2ndh dhana bhāva
iabbr 1stH Tanū bhāva
iabbr 1sth tanū bhāva
iabbr 12thL Vyayeśa
iabbr 12thl vyayeśa
iabbr 12thH Vyaya bhāva
iabbr 12thh vyaya bhāva
iabbr 11thL Lābheśa
iabbr 11thl lābheśa
iabbr 11thH Āya bhāva
iabbr 11thh āya bhāva
iabbr 10thL Karmeśa
iabbr 10thl karmeśa
iabbr 10thH Karma bhāva
iabbr 10thh karma bhāva
cnoreabbr csn set cscopequickfix=
cnoreabbr cso set cscopequickfix=s-,c-,d-,i-,t-,e-
cnoreabbr csh cs help
cnoreabbr css cs show
cnoreabbr csr cs reset
cnoreabbr csk cs kill
cnoreabbr csf cs find
cnoreabbr csa cs add
let &cpo=s:cpo_save
unlet s:cpo_save
set autowrite
set background=dark
set backspace=2
set clipboard=unnamedplus
set complete=.,w,b,u,t,i,kspell
set completefunc=youcompleteme#CompleteFunc
set completeopt=preview,menuone
set cpoptions=aAceFsB
set cscopetag
set fileencodings=ucs-bom,utf-8,default,latin1
set guifont=Source\ Code\ Pro\ 10
set guioptions=aegiLt
set helplang=en
set hlsearch
set laststatus=2
set lazyredraw
set nomodeline
set printoptions=paper:a4
set ruler
set runtimepath=~/.vim,~/.vim/bundle/Vundle.vim,~/.vim/bundle/nerdtree,~/.vim/bundle/nerdcommenter,~/.vim/bundle/lightline.vim,~/.vim/bundle/vim-gitbranch,~/.vim/bundle/Conque-GDB,~/.vim/bundle/taglist.vim,~/.vim/bundle/YouCompleteMe,/var/lib/vim/addons,/usr/share/vim/vimfiles,/usr/share/vim/vim80,/usr/share/vim/vimfiles/after,/var/lib/vim/addons/after,~/.vim/after,~/.vim/bundle/Vundle.vim,~/.vim/bundle/Vundle.vim/after,~/.vim/bundle/nerdtree/after,~/.vim/bundle/nerdcommenter/after,~/.vim/bundle/lightline.vim/after,~/.vim/bundle/vim-gitbranch/after,~/.vim/bundle/Conque-GDB/after,~/.vim/bundle/taglist.vim/after,~/.vim/bundle/YouCompleteMe/after
set shortmess=filnxtToOc
set noshowmode
set spellfile=~/.vimsl.utf-8.add
set statusline=%F%m%r%h%w
set suffixes=.bak,~,.swp,.o,.info,.aux,.log,.dvi,.bbl,.blg,.brf,.cb,.ind,.idx,.ilg,.inx,.out,.toc
set synmaxcol=255
set tabline=%!lightline#tabline()
set tags=~/.tags
set termguicolors
set textwidth=79
set visualbell
" vim: set ft=vim :
