
(setq win-context-menu-program "c:/tools/WinContextMenu_x64.exe")

(defun win-context-menu (filenames)
  "Display Windows context menu for specified files"

  (let ( (quoted_names (mapcar (lambda (x) (format "\"%s\"" x) ) filenames) )
         ;;         (args (mapconcat 'identity argslist " "))
         )
    (defalias 'prog-fn (apply-partially 'start-process-shell-command "WinContextMenu" "*WinContextMenu*" win-context-menu-program) )
    (apply 'prog-fn quoted_names)
    )
  )

(defun show-win-context-menu ()
  "Display Windows context menu on selected files"
  (interactive)

  (if (eq system-type 'windows-nt)
      (let* ((files (dired-get-marked-files))
             (files (if (null files)
                        (list (dired-current-directory) )
                      files)))

        (win-context-menu files) )
    )
  )

(defun show-win-context-menu-current-buffer ()
    (interactive)
    (win-context-menu (list buffer-file-name)))

(define-key dired-mode-map "c"
  'show-win-context-menu)

;(define-key dired-mode-map "\C-cc"
;  'show-win-context-menu)
